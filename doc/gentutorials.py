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
	makeFileInfo('tutorial_callable')
]

def writeFile(fileName, content) :
	with codecs.open(fileName, "w", "utf-8") as file :
		file.write(str(content))

def readFile(fileName) :
	with codecs.open(fileName, "r", "utf-8") as file :
		return file.read()

def doProcessFile(inputFile, outputFile) :
	lineList = readFile(inputFile).splitlines()
	outputLineList = []
	state = State.none
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
				outputLineList.append("```")
				outputLineList.append("")
			else :
				state = State.code
				outputLineList.append("")
				outputLineList.append("```c++")
		else :
			if state != State.none :
				outputLineList.append(line)
	writeFile(outputFile, "\n".join(outputLineList))

def doMain() :
	for fileInfo in fileInfoList :
		doProcessFile(fileInfo['input'], fileInfo['output'])

doMain()
