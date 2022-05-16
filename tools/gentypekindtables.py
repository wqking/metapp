import openpyxl 

excelFile = '../tests/docsrc/tables.xlsx'
outputPath = '../tests/docsrc/inc/'
outputTableList = [
	{
		'columnIndexList' : [ 0, 1, 2 ],
		'fileName' : outputPath + 'inc_typekind_cpp_type.md'
	},
	{
		'columnIndexList' : [ 0, 4 ],
		'fileName' : outputPath + 'inc_typekind_meta_interface.md'
	},
	{
		'columnIndexList' : [ 0, 5, 6 ],
		'fileName' : outputPath + 'inc_typekind_up_type.md'
	},
	{
		'columnIndexList' : [ 0, 3 ],
		'fileName' : outputPath + 'inc_typekind_cast.md'
	},
]

def loadSheetData(sheet) :
	rowList = []
	columnCount = 0
	for row in sheet :
		rowData = []
		for cell in row :
			value = str(cell.value)
			value = value.replace("\n", "<br />")
			rowData.append(value)
		rowList.append(rowData)
		columnCount = len(rowData)
	result = {
		'rowList' : rowList,
		'rowCount' : len(rowList),
		'columnCount' : columnCount,
	}
	return result

def ensureTextWidth(width, text, fill) :
	while len(text) < width :
		text += fill
	return text

def generateMarkDownTable(sheetData, columnIndexList) :
	columnWidthList = []
	for _ in range(sheetData['columnCount']) :
		columnWidthList.append(10)
	for columnIndex in columnIndexList :
		for rowIndex in range(0, sheetData['rowCount']) :
			value = str(sheetData['rowList'][rowIndex][columnIndex])
			if len(value) > columnWidthList[columnIndex] :
				columnWidthList[columnIndex] = len(value)
	lineList = []
	for rowIndex in range(0, sheetData['rowCount']) :
		line = '|'
		for columnIndex in columnIndexList :
			value = str(sheetData['rowList'][rowIndex][columnIndex])
			line += ensureTextWidth(columnWidthList[columnIndex], value, ' ')
			line += '|'
		lineList.append(line)
		if rowIndex == 0 :
			line = '|'
			for columnIndex in columnIndexList :
				line += ensureTextWidth(columnWidthList[columnIndex], '', '-')
				line += '|'
			lineList.append(line)
	return "\n".join(lineList)

def doMain() :
	workBook = openpyxl.load_workbook(filename = excelFile)
	workBook.active = 0
	sheet = workBook.active
	sheetData = loadSheetData(sheet)
	for config in outputTableList :
		tableText = generateMarkDownTable(sheetData, config['columnIndexList'])
		with open(config['fileName'], 'w') as file:
			file.write(tableText)

doMain()
