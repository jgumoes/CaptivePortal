"""
  a script to compile and minify the config webpage into one file.
  TODO: minify the pages
"""
import regex as re

# guard clauses for missing dependancies

def notImported(page):
  print("missing module: file not minified")

try:
  from rjsmin import jsmin
except ModuleNotFoundError:
  jsmin = notImported

try:
  from htmlmin import minify as minifyHtml
except ModuleNotFoundError:
  minifyHtml = notImported

try:
  from csscompressor import compress as minifyCss
except ModuleNotFoundError:
  minifyCss = notImported

# actual functions that do stuff

def openFile(filename: str):
  filePath = f".\\webpage\\{filename}"
  with open(filePath) as fileObj:
    openFile = fileObj.read()
  return openFile

def replaceStyles(webpage: str):
  links = re.findall(f"<link rel=[\'\"]stylesheet[\'\"] href=\"(.*)\">", webpage) # find the stylesheet links
  for link in links:
    cssFile = minifyCss(f"<style>{openFile(link)}</style>")
    webpage = re.sub(f"<link rel=[\'\"]stylesheet[\'\"] href=\"{link}\">", cssFile, webpage)
  return webpage

def replaceJavascript(webpage: str):
  scripts = re.findall(f"<script src=[\'\"](.*)[\'\"]>[.\s]*</script>", webpage)
  for script in scripts:
    jsFile = f"<script>\n{openFile(script)}\n</script>"
    jsFile = jsmin(jsFile)
    webpage = re.sub(f"<script src=[\'\"]{script}[\'\"]>[.\s]*</script>", jsFile, webpage)
  return webpage

# compile the webpage
webpage = openFile("config.html")
webpage = replaceStyles(webpage)
webpage = replaceJavascript(webpage)
webpage = minifyHtml(webpage)

# save the compiled webpage
with open(".\\data\\config.html", "w") as newFile:
  newFile.write(webpage)
