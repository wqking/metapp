import enum
import re
import codecs

@enum.unique
class State(enum.IntEnum) :
	none = 0
	desc = 1
	code = 2

sourcePath = '../tests/tutorial/'
docPath = './tutorial/'

def makeFileInfo(fileName) :
	return {
		'input' : sourcePath + fileName + '.cpp',
		'output' : docPath + fileName + '.md',
	}

fileInfoList = [
	makeFileInfo('tutorial_callable'),
	makeFileInfo('tutorial_metaclass'),
]

def writeFile(fileName, content) :
	with codecs.open(fileName, "w", "utf-8") as file :
		file.write(str(content))

def readFile(fileName) :
	with codecs.open(fileName, "r", "utf-8") as file :
		return file.read()

def doTabToSpace(text) :
	def callback(m) :
		return '    ' * len(m.group(1))
	text = re.sub(r'^(\t+)', callback, text)
	return text

def doProcessCodeLines(lineList, beginIndex, endIndex) :
	# find out the redundant leading spaces to remove from (unindent)
	leadingSpaceLength = None
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

def doProcessFile(inputFile, outputFile) :
	lineList = readFile(inputFile).splitlines()
	outputLineList = []
	state = State.none
	codeBeginIndex = 0
	for line in lineList :
		matches = re.match(r'\s*[\/\*]*\s*(\w+)\s*[\/\*]*\s*', line)
		command = None
		if matches is not None :
			command = matches.group(1).lower()
		if command == 'desc' :
			if state == State.desc :
				state = State.none
				outputLineList.append("")
			else :
				state = State.desc
				outputLineList.append("")
		elif command == 'code' :
			if state == State.code :
				state = State.none
				doProcessCodeLines(outputLineList, codeBeginIndex, len(outputLineList) - 1)
				outputLineList.append("```")
				outputLineList.append("")
			else :
				state = State.code
				outputLineList.append("")
				outputLineList.append("```c++")
				codeBeginIndex = len(outputLineList)
		else :
			if state != State.none :
				outputLineList.append(line)
	writeFile(outputFile, "\n".join(outputLineList))

def doMain() :
	for fileInfo in fileInfoList :
		doProcessFile(fileInfo['input'], fileInfo['output'])

doMain()
