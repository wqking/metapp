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
import pathlib

forceGenerateAll = False
sourcePath = '../tests/docsrc'
docPath = '../doc'

fileConfigs = {
	'readme' : {
		'targetFileName' : '../readme.md',
		#'generateToc' : False,
	},

	'list_all' : {
		'tocMinHeadings' : 2
	}
}

def isFileUpToDate(sourceFile, targetFile) :
	if forceGenerateAll :
		return False
	if not os.path.exists(targetFile) :
		return False
	if os.path.getmtime(sourceFile) > os.path.getmtime(targetFile) :
		return False
	return True

def getDictValue(dict, key, default = None) :
	if dict != None and key in dict :
		return dict[key]
	return default

def isWindows() :
	return sys.platform.startswith('win')

def normalizeCommand(command) :
	return shlex.split(command, posix = not isWindows())

def doPostProcessMarkdown(fileName, config) :
	#header = [ "<!--Auto generated file, don't modify this file.-->", '' ]
	# The invisible text trick is from here
	# https://stackoverflow.com/questions/4823468/comments-in-markdown
	# Ideally we should use the above commented format <!-- -->,
	# but my markdown previewer doens't support it when it's the first line...
	header = [ "[//]: # (Auto generated file, don't modify this file.)", '' ]
	lineList = cpp2md.readLines(fileName)
	lineList[:] = [ cpp2md.textLeadingTabToSpace(line) for line in lineList ]
	lineList = header + lineList
	cpp2md.writeLines(fileName, lineList)

	if config['generateToc'] :
		command = 'perl addtoc2md.pl --max-level=4 --min-headings=%d "%s"' % (config['tocMinHeadings'], fileName)
		command = normalizeCommand(command)
		subprocess.run(command)

def doGetFileConfig(sourceFile) :
	matches = re.search(r'doc_([^\\\/]+)\.(\w+)$', sourceFile)
	if matches is None :
		print("Internal error %s" % (sourceFile))
		return None
	fileName = matches.group(1)
	fileType = matches.group(2)
	relativePath = os.path.abspath(sourceFile)[len(sourcePath) : ]
	relativePath = re.sub(r'^[\\\/]', '', relativePath)
	relativePath = os.path.dirname(relativePath)
	targetFileName = os.path.join(docPath, relativePath, fileName + '.md')
	specialConfig = getDictValue(fileConfigs, fileName)
	targetFileName = getDictValue(specialConfig, 'targetFileName', targetFileName)
	generateToc = getDictValue(specialConfig, 'generateToc', True)
	tocMinHeadings = getDictValue(specialConfig, 'tocMinHeadings', 6)
	os.makedirs(os.path.dirname(targetFileName), exist_ok = True)
	config = {
		'fileName' : fileName,
		'fileType' : fileType,
		'targetFileName' : targetFileName,
		'generateToc' : generateToc,
		'tocMinHeadings' : tocMinHeadings,
	}
	return config

def doProcessFile(sourceFile) :
	config = doGetFileConfig(sourceFile)
	if config is None :
		return

	targetFileName = config['targetFileName']
	fileType = config['fileType']

	if isFileUpToDate(sourceFile, targetFileName) :
		print("%s is up to date, skip." % (targetFileName))
		return
	print("Generate %s" % (targetFileName))
	if fileType == 'cpp' :
		cpp2md.extractMarkdownFromCpp(sourceFile, targetFileName)
	elif fileType in [ 'md' ] :
		shutil.copy(sourceFile, targetFileName)
	else :
		print("Unknow file type: %s" % (fileType))
		return
	if targetFileName.endswith('.md') :
		doPostProcessMarkdown(targetFileName, config)

def checkGenerateTables() :
	excelFileName = os.path.join(sourcePath, 'tables.xlsx')
	incFileName = os.path.join(sourcePath, 'inc/inc_typekind_cpp_type.md')
	if isFileUpToDate(excelFileName, incFileName) :
		return
	print("Re-generate markdown tables.")
	subprocess.run('python gentypekindtables.py')
	docFileName = os.path.join(sourcePath, 'metatypes/doc_list_all.cpp')
	pathlib.Path(docFileName).touch()

def doMain() :
	global sourcePath, docPath

	sourcePath = os.path.abspath(sourcePath)
	docPath = os.path.abspath(docPath)

	pattern = os.path.join(sourcePath, '**/doc_*.*')
	sourceFileList = glob.glob(pattern, recursive = True)

	checkGenerateTables()

	for sourceFile in sourceFileList :
		doProcessFile(sourceFile)

doMain()
