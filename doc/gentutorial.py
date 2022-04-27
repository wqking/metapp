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
	with codecs.open(fileName, "w", "utf-8") as file :
		file.write(str(content))

def readFile(fileName) :
	with codecs.open(fileName, "r", "utf-8") as file :
		return file.read()

def doTabToSpace(text) :
	def callback(m) :
		return '  ' * len(m.group(1))
	text = re.sub(r'^(\t+)', callback, text)
	return text

def doProcessCodeLines(lineList, beginIndex, endIndex, unindent) :
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
		line = doTabToSpace(line)
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
		line = line.lstrip()
		resultList.append(line)
	resultList.append('')
	return resultList

def doConvertCodeBlock(block) :
	lineList = block['lineList']
	resultList = [] + lineList
	doRemoveBlankCodeLines(resultList, 0, len(resultList) - 1)
	doProcessCodeLines(resultList, 0, len(resultList) - 1, not block['hasParent'])
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

def doProcessFile(inputFile, outputFile) :
	lineList = readFile(inputFile).splitlines()
	outputLineList = []
	blockList = doParseBlocks(lineList)
	blockList = doRemoveEmptyBlocks(blockList)
	blockList = doCombineSingleLineDescBlocks(blockList)
	for block in blockList :
		outputLineList += doConvertBlock(block)
	writeFile(outputFile, "\n".join(outputLineList))

def doMain() :
	if len(sys.argv) != 3 :
		print('Usage: python gentutorial.py INPUT_FILE OUTPUT_FILE')
		return
	inputFile = sys.argv[1]
	outputFile = sys.argv[2]
	doProcessFile(inputFile, outputFile)

doMain()
