#!/usr/bin/env python

# Tool markdown-toc
# 
# Copyright (C) 2022 Wang Qi (wqking)
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#   http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import glob
import argparse
import traceback
import sys
import re
import zlib

def writeFile(fileName, content) :
	with open(fileName, 'w') as file:
		file.write(str(content))

def readFile(fileName) :
	with open(fileName, 'r') as file:
		return file.read()

def readLines(fileName) :
	return (readFile(fileName) + "\n").splitlines()

def writeLines(fileName, lineList) :
	writeFile(fileName, "\n".join(lineList))

blockType_content = 0
blockType_heading = 1
blockType_anchor = 2
blockType_tocPlaceholder = 3
blockType_ignore = 4

class MarkdownParser :
	def __init__(self, config, lineList) :
		self._config = config
		self._lineList = lineList
		self._resultLineList = []
		self._blockList = []
		self._currentBlock = None
		self._inCode = False
		self._inToc = False

	def process(self) :
		self._doParse()

		if self._config['action'] == 'update' :
			self._doUpdate()
		else :
			self._doRemove()
		return self._resultLineList

	def _doUpdate(self) :
		if not self._shouldAddToc() :
			self._doRemove()
			return
		self._generateAnchors()
		self._createTocBlock()
		self._generateResult(True)

	def _doRemove(self) :
		self._generateResult(False)

	def _generateResult(self, includeToc) :
		for block in self._blockList :
			if block['type'] == blockType_ignore :
				continue
			if not includeToc :
				if block['type'] in [ blockType_anchor, blockType_tocPlaceholder ] :
					continue
			for line in block['lineList'] :
				self._resultLineList.append(line)

	def _createTocBlock(self) :
		tocLineList = self._generateTocLineList()
		tocBlockList = self._findTocPlaceholderBlocks()
		if len(tocBlockList) > 0 :
			tocBlockList[0]['lineList'] = tocLineList
			for i in range(1, len(tocBlockList)) :
				tocBlockList[i]['type'] = blockType_ignore
			return
		tocBlock = {
			'type' : blockType_tocPlaceholder,
			'lineList' : tocLineList
		}
		tocIndex = 0
		if not self._config['front'] :
			for i in range(len(self._blockList)) :
				if self._blockList[i]['type'] == blockType_heading :
					tocIndex = i + 1
					for k in range(i) :
						if self._blockList[k]['type'] == blockType_anchor :
							self._blockList[k]['type'] = blockType_ignore
					break
		self._blockList.insert(tocIndex, tocBlock)

	def _generateTocLineList(self) :
		tocLineList = self._generateToc()
		if self._config['before'] is not None :
			lines = self._config['before'].splitlines()
			tocLineList = lines + tocLineList
		if self._config['after'] is not None :
			lines = self._config['after'].splitlines()
			tocLineList = tocLineList + lines
		tocLineList.insert(0, '<!--begintoc-->')
		tocLineList.append('<!--endtoc-->')
		return tocLineList

	def _generateToc(self) :
		lineList = []
		indent = '  '
		depth = 0
		previousLevel = -1
		for block in self._blockList :
			if not self._isValidHeading(block) :
				continue
			level = block['level']
			if previousLevel < 0 :
				previousLevel = level
			elif previousLevel != level :
				if previousLevel < level :
					depth += 1
				else :
					depth -= 1
					if depth < 0 :
						depth = 0
				previousLevel = level
			text = indent * depth
			text += '- [%s](#%s)' % (block['text'], block['id'])
			lineList.append(text)
		return lineList

	def _generateAnchors(self) :
		newBlockList = []
		length = len(self._blockList)
		i = -1
		while True :
			i += 1
			if i >= length :
				break
			block = self._blockList[i]
			if block['type'] == blockType_anchor :
				if (i + 1 < length) and (self._blockList[i + 1]['type'] == blockType_heading) :
					nextBlock = self._blockList[i + 1]
					nextBlock['id'] = block['id']
					newBlockList.append(block)
					newBlockList.append(nextBlock)
					i += 1
				continue
			if (block['type'] == blockType_heading) and (block['id'] is None) :
				block['id'] = self._createAnchorIdFromText(block['text'])
				newAnchorBlock = {
					'type' : blockType_anchor,
					'lineList' : [ '<a id="%s"></a>' % (block['id']) ],
					'id' : block['id']
				}
				newBlockList.append(newAnchorBlock)
			newBlockList.append(block)
		self._blockList = newBlockList

	def _createAnchorIdFromText(self, text) :
		text = re.sub(r'\s', '', text)
		text = text.lower()
		crc32 = zlib.crc32(bytes(text, 'utf-8'))
		while True :
			found = False
			id = 'mdtoc_' + format(crc32, 'x')
			for block in self._blockList :
				if ('id' in block) and (block['id'] == id) :
					found = True
					break
			if not found :
				return id
			crc32 += 1

	def _isValidHeading(self, block) :
		if block['type'] == blockType_heading :
			level = block['level']
			if level >= self._config['minLevel'] and level <= self._config['maxLevel'] :
				return True
		return False

	def _shouldAddToc(self) :
		headingsCount = 0
		for block in self._blockList :
			if self._isValidHeading(block) :
				headingsCount += 1
		if headingsCount < self._config['minHeadings'] :
			return False
		return True

	def _doParse(self) :
		self._currentBlock = None
		self._inCode = False

		for line in self._lineList :
			self._doParseLine(line)

	def _doParseLine(self, line) :
		if line.startswith('```') :
			self._inCode = not self._inCode
			self._requireBlock(blockType_content, line)
			return

		if self._inCode :
			self._requireBlock(blockType_content, line)
			return

		trimmed = line.strip()
		if trimmed == '<!--toc-->' :
			if len(self._findTocPlaceholderBlocks()) == 0 :
				self._requireBlock(blockType_tocPlaceholder, line, newBlock = True)
			return

		if trimmed == '<!--begintoc-->' :
			self._inToc = True
			for block in self._findTocPlaceholderBlocks() :
				block['type'] == blockType_ignore
			return

		if trimmed == '<!--endtoc-->' :
			self._inToc = False
			return

		if self._inToc :
			return

		matches = re.match(r'^(#+)\s+(.*)$', line)
		if matches is not None :
			tag = matches[1]
			text = matches[2]
			block = self._requireBlock(blockType_heading, line, newBlock = True)
			block['level'] = len(tag)
			block['text'] = text
			block['id'] = None
			return

		matches = re.match(r'^\s*<a\s+id="(mdtoc_[\w\d]+)">\s*<\/a>$', line)
		if matches is not None :
			id = matches[1]
			block = self._requireBlock(blockType_anchor, line, newBlock = True)
			block['id'] = id
			return

		self._requireBlock(blockType_content, line)

	def _findTocPlaceholderBlocks(self) :
		resultList = []
		for block in self._blockList :
			if block['type'] == blockType_tocPlaceholder :
				resultList.append(block)
		return resultList

	def _requireBlock(self, blockType, line, newBlock = False) :
		if newBlock :
			self._currentBlock = None
		if (self._currentBlock is None) or (self._currentBlock['type'] != blockType) :
			self._currentBlock = {
				'type' : blockType,
				'lineList' : [],
			}
			self._blockList.append(self._currentBlock)
		self._currentBlock['lineList'].append(line)
		return self._currentBlock

class Application :
	def __init__(self) :
		self._config = {
			'action' : 'update',
			'before' : None,
			'after' : None,
			'minHeadings' : 6,
			'minLevel' : 2,
			'maxLevel' : 3,
			'front' : 0
		}
		self._filePatterns = []
		self._excludePatterns = []

	def run(self) :
		try :
			self._parseCommandLine(sys.argv[1:])
			self._doRun()
		except Exception as e:
			traceback.print_exc()
			
	def _unescapeText(self, text) :
		if text is None :
			return text;
		text = text.replace("\\n", "\n")
		return text

	def _doRun(self) :
		for filePattern in self._filePatterns :
			self._doOnFilePattern(filePattern)

	def _doOnFilePattern(self, filePattern) :
		fileList = glob.glob(filePattern, recursive = True)
		for file in fileList :
			self._doOnFile(file)

	def _doOnFile(self, file) :
		if not self._canProcessFile(file) :
			return
		fileLines = readLines(file)
		lineList = self._doProcessLines(fileLines)
		#print("\n".join(lineList))
		#return
		if not self._isSameList(fileLines, lineList) :
			writeLines(file, lineList)

	def _doProcessLines(self, lineList) :
		parser = MarkdownParser(self._config, lineList)
		return parser.process()
				
	def _canProcessFile(self, file) :
		for exclude in self._excludePatterns :
			if exclude in file :
				return False
		return True

	def _isSameList(self, a, b) :
		if len(a) != len(b) :
			return False
		for i in range(len(a)) :
			if a[i] != b[i] :
				return False
		return True

	def _parseCommandLine(self, commandLineArguments) :
		parser = argparse.ArgumentParser(add_help = False)
		parser.add_argument('--help', action = 'store_true', help = 'Show help message')
		parser.add_argument('-h', action = 'store_true', dest = 'help', help = 'Show help message')
		parser.add_argument(
			'action',
			help = "The action, can be update or remove",
			default = self._config['action'],
			choices = [ 'update', 'remove' ]
		)
		parser.add_argument(
			'--file',
			action = 'append',
			required = True,
			help = "The markdown file patterns, can have path and wildcard"
		)
		parser.add_argument(
			'--before',
			required = False,
			help = "The text added before the TOC block",
			default = self._config['before']
		)
		parser.add_argument(
			'--after',
			required = False,
			help = "The text added after the TOC block",
			default = self._config['after']
		)
		parser.add_argument(
			'--min-headings',
			type = int,
			required = False,
			help = "The text added after the TOC block",
			default = self._config['minHeadings']
		)
		parser.add_argument(
			'--min-level',
			type = int,
			required = False,
			help = "The text added after the TOC block",
			default = self._config['minLevel']
		)
		parser.add_argument(
			'--max-level',
			type = int,
			required = False,
			help = "The text added after the TOC block",
			default = self._config['maxLevel']
		)
		parser.add_argument(
			'--front',
			type = int,
			required = False,
			help = "0 to put the TOC after the first valid heading. 1 to put the TOC in front of the document.",
			default = self._config['front']
		)
		parser.add_argument(
			'--exclude',
			action = 'append',
			required = False,
			help = "The patterns to exclude, can not have wildcard"
		)

		options = parser.parse_args(commandLineArguments)
		options = vars(options)

		if options['help'] :
			self._showUsage(parser)
			return False

		self._config['action'] = options['action']
		self._config['before'] = self._unescapeText(options['before'])
		self._config['after'] = self._unescapeText(options['after'])
		self._config['minHeadings'] = options['min_headings']
		self._config['minLevel'] = options['min_level']
		self._config['maxLevel'] = options['max_level']
		self._config['front'] = options['front']

		self._filePatterns = options['file']
		self._excludePatterns = options['exclude']
		if self._excludePatterns is None :
			self._excludePatterns = []

	def _showUsage(self, parser) :
		parser.print_help()

if __name__ == "__main__" :
	Application().run()
