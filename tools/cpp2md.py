# metapp library

# Copyright (C) 2022 Wang Qi (wqking)

# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at

#   http://www.apache.org/licenses/LICENSE-2.0

# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import enum
import re
import codecs
import sys

@enum.unique
class BlockType(enum.IntEnum) :
	desc = 0
	singleLineDesc = 1
	code = 2

def writeFile(fileName, content) :
	with open(fileName, 'w') as file:
		file.write(str(content))

def readFile(fileName) :
	with open(fileName, 'r') as file:
		return file.read()

def readLines(fileName) :
	return readFile(fileName).splitlines()

def writeLines(fileName, lineList) :
	writeFile(fileName, "\n".join(lineList))

def textLeadingTabToSpace(text) :
	def callback(m) :
		return '  ' * len(m.group(1))
	text = re.sub(r'^(\t+)', callback, text)
	return text

def fileLeadingTabToSpace(fileName) :
	lineList = readLines(fileName)
	lineList[:] = [ textLeadingTabToSpace(line) for line in lineList ]
	writeLines(fileName, lineList)

def doFormatCodeLines(lineList, beginIndex, endIndex, unindent) :
	# find out the redundant leading spaces to remove from (unindent)
	leadingSpaceLength = None
	if unindent :
		index = beginIndex
		while index <= endIndex :
			line = lineList[index]
			matches = re.match(r'^(\s*)\S+', line)
			if matches is not None :
				leadingLength = len(matches.group(1))
				if (leadingSpaceLength is None) or (leadingLength < leadingSpaceLength) :
					leadingSpaceLength = leadingLength
			index += 1

	if leadingSpaceLength is None :
		leadingSpaceLength = 0
	index = beginIndex
	while index <= endIndex :
		line = lineList[index]
		if leadingSpaceLength > 0 :
			line = line[leadingSpaceLength:]
		#line = textLeadingTabToSpace(line)
		lineList[index] = line
		index += 1

def doFormatDescLines(lineList, beginIndex, endIndex) :
	index = beginIndex
	inCodeBlock = False
	while index <= endIndex :
		line = lineList[index]
		if line.startswith("```") :
			inCodeBlock = not inCodeBlock
		else :
			canRemoveLeadingSpaces = True
			if inCodeBlock :
				canRemoveLeadingSpaces = False
			elif re.match(r'^\s*[\-\*].*', line) is not None :
				canRemoveLeadingSpaces = False
			if canRemoveLeadingSpaces :
				line = line.lstrip()
		#line = textLeadingTabToSpace(line)
		lineList[index] = line
		index += 1

def trim(text) :
	return text.strip(" \t\r\n")

def isEmptyText(text) :
	return text.strip(" \t\r\n") == ''

def doRemoveBlankCodeLines(lineList, beginIndex, endIndex) :
	index = beginIndex
	while index <= endIndex :
		line = trim(lineList[index])
		if line != '' :
			break
		del lineList[index]
		endIndex -= 1
	beginIndex = index
	index = endIndex
	while index >= beginIndex :
		line = trim(lineList[index])
		if line != '' :
			break
		del lineList[index]
		index -= 1

class BlockParser :
	def __init__(self) :
		self._blockList = []
		self._blockStack = []
		self._hasParent = False

	def process(self, lineList) :
		for line in lineList :
			matches = re.match(r'\s*//\s*desc\s*(.*)\s*', line, re.I)
			if matches is not None :
				text = matches.group(1)
				self._processCommand(BlockType.singleLineDesc)
				self._processLine(text)
				self._processCommand(BlockType.singleLineDesc)
				continue
			matches = re.match(r'\s*[\/\*]*\s*(\w+)\s*[\/\*]*\s*', line)
			command = None
			if matches is not None :
				command = matches.group(1).lower()
			if command == 'desc' :
				self._processCommand(BlockType.desc)
			elif command == 'code' :
				self._processCommand(BlockType.code)
			else :
				self._processLine(line)
	
		return self._blockList

	def _getCurrentBlock(self) :
		if len(self._blockStack) > 0 :
			return self._blockStack[-1]
		return None

	def _beginBlock(self, type) :
		block = {
			'type' : type,
			'lineList' : [],
			'hasParent' : self._hasParent
		}
		self._blockStack.append(block)
		self._blockList.append(block)

	def _endBlock(self) :
		if len(self._blockStack) == 0 :
			return
		self._blockStack.pop()
		currentBlock = self._getCurrentBlock()
		if currentBlock is not None :
			self._blockStack.pop()
			self._beginBlock(currentBlock['type'])

	def _processCommand(self, type) :
		currentBlock = self._getCurrentBlock()
		if currentBlock is None :
			self._hasParent = False
			self._beginBlock(type)
		elif currentBlock['type'] == type :
			if type != BlockType.desc :
				self._hasParent = False
			self._endBlock()
		else :
			self._hasParent = True
			self._beginBlock(type)
	
	def _processLine(self, line) :
		currentBlock = self._getCurrentBlock()
		if currentBlock is not None :
			currentBlock['lineList'].append(line)

def doParseBlocks(lineList) :
	return BlockParser().process(lineList)

def doConvertDescBlock(block) :
	lineList = block['lineList']
	resultList = []
	for line in lineList :
		resultList.append(line)
	doFormatDescLines(resultList, 0, len(resultList) - 1)
	resultList.append('')
	return resultList

def doConvertCodeBlock(block) :
	lineList = block['lineList']
	resultList = [] + lineList
	doRemoveBlankCodeLines(resultList, 0, len(resultList) - 1)
	doFormatCodeLines(resultList, 0, len(resultList) - 1, not block['hasParent'])
	resultList = ([ "```c++" ] + resultList + [ "```", "" ])
	return resultList

def doConvertBlock(block) :
	if block['type'] == BlockType.desc :
		return doConvertDescBlock(block)
	if block['type'] == BlockType.code :
		return doConvertCodeBlock(block)
	return block['lineList']

def doRemoveEmptyBlocks(blockList) :
	resultList = []
	for block in blockList :
		for line in block['lineList'] :
			if not isEmptyText(line) :
				resultList.append(block)
				break
	return resultList

def doCombineSingleLineDescBlocks(blockList) :
	resultList = []
	previousSingleLineBlock = None
	for block in blockList :
		if block['type'] != BlockType.singleLineDesc :
			previousSingleLineBlock = None
			resultList.append(block)
			continue
		if previousSingleLineBlock is None :
			previousSingleLineBlock = block.copy()
			previousSingleLineBlock['type'] = BlockType.desc
			resultList.append(previousSingleLineBlock)
		else :
			previousSingleLineBlock['lineList'] += block['lineList']
	return resultList

def extractMarkdownFromCpp(inputFile, outputFile) :
	lineList = readLines(inputFile)
	outputLineList = []
	blockList = doParseBlocks(lineList)
	blockList = doRemoveEmptyBlocks(blockList)
	blockList = doCombineSingleLineDescBlocks(blockList)
	for block in blockList :
		outputLineList += doConvertBlock(block)
	writeLines(outputFile, outputLineList)

def doMain() :
	if len(sys.argv) != 3 :
		print('Usage: python cpp2md.py INPUT_FILE OUTPUT_FILE')
		return
	inputFile = sys.argv[1]
	outputFile = sys.argv[2]
	print(inputFile)
	extractMarkdownFromCpp(inputFile, outputFile)

if __name__ == "__main__" :
	doMain()
