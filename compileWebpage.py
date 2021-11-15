"""
  a script to compile and minify the config webpage into one file.
  TODO: minify the pages
"""
import regex as re

def notImported(page):
  print("missing module: file not minified")

try:
  from rjsmin import jsmin
except ModuleNotFoundError:
  jsmin = notImported

def openFile(filename: str):
  filePath = f".\\webpage\\{filename}"
  return open(filePath).read()

def replaceStyles(webpage: str):
  links = re.findall(f"<link rel=[\'\"]stylesheet[\'\"] href=\"(.*)\">", webpage) # find the stylesheet links
  for link in links:
    cssFile = f"<style>{openFile(link)}</style>"  # TODO: minify
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

# save the compiled webpage
newFile = open(".\\data\\config.html", "w")
newFile.write(webpage)
