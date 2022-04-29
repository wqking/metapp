from distutils import extension
from pydoc import doc
from cpp2md import extractMarkdownFromCpp
import os
import glob
import re

sourcePath = '../tests/docsrc'
docPath = '../doc'

def doProcessFile(sourceFile) :
	matches = re.search(r'doc_([^\\\/]+)\.(\w+)$', sourceFile)
	if matches is None :
		print("Internal error %s" % (sourceFile))
		return
	fileName = matches.group(1)
	extension = matches.group(2)
	relativePath = os.path.abspath(sourceFile)[len(sourcePath) : ]
	relativePath = re.sub(r'^[\\\/]', '', relativePath)
	relativePath = os.path.dirname(relativePath)
	targetPath = os.path.join(docPath, relativePath)
	targetFileName = os.path.join(targetPath, fileName + '.md')
	os.makedirs(targetPath, exist_ok = True)
	print(sourceFile)
	extractMarkdownFromCpp(sourceFile, targetFileName)
	#print(fileName, extension, targetFileName)

def doMain() :
	global sourcePath, docPath

	sourcePath = os.path.abspath(sourcePath)
	docPath = os.path.abspath(docPath)

	pattern = os.path.join(sourcePath, '**/doc_*.*')
	sourceFileList = glob.glob(pattern, recursive = True)

	for sourceFile in sourceFileList :
		doProcessFile(sourceFile)

doMain()
