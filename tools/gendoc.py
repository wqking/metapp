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

import cpp2md
import os
import sys
import glob
import re
import shutil
import subprocess
import shlex

forceGenerateAll = not False
sourcePath = '../tests/docsrc'
docPath = '../doc'

def isFileUpToDate(sourceFile, targetFile) :
	if forceGenerateAll :
		return False
	if not os.path.exists(targetFile) :
		return False
	if os.path.getmtime(sourceFile) > os.path.getmtime(targetFile) :
		return False
	return True

def isWindows() :
	return sys.platform.startswith('win')

def normalizeCommand(command) :
	return shlex.split(command, posix = not isWindows())

def doPostProcessMarkdown(fileName) :
	cpp2md.fileLeadingTabToSpace(fileName)

	command = 'perl addtoc2md.pl --max-level=4 "%s"' % (fileName)
	command = normalizeCommand(command)
	subprocess.run(command)

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
	if isFileUpToDate(sourceFile, targetFileName) :
		print("%s is up to date, skip." % (targetFileName))
		return
	print("Generate %s" % (targetFileName))
	if extension == 'cpp' :
		cpp2md.extractMarkdownFromCpp(sourceFile, targetFileName)
	elif extension in [ 'md' ] :
		shutil.copy(sourceFile, targetFileName)
	else :
		print("Unknow file type: %s" % (extension))
		return
	if targetFileName.endswith('.md') :
		doPostProcessMarkdown(targetFileName)

def doMain() :
	global sourcePath, docPath

	sourcePath = os.path.abspath(sourcePath)
	docPath = os.path.abspath(docPath)

	pattern = os.path.join(sourcePath, '**/doc_*.*')
	sourceFileList = glob.glob(pattern, recursive = True)

	for sourceFile in sourceFileList :
		doProcessFile(sourceFile)

doMain()
