"""
  a script to compile and minify the config webpage into one file.
  TODO: minify the pages
"""
import regex as re

def openFile(filename: str):
  filePath = f".\\webpage\\{filename}"
  return open(filePath).read()

# def listLinkedFiles(rel, webpage):
#   # TODO: refactor to return a tuple of lists [rel, href]
#   # return re.findall(f"<link rel=\"(.*)\" href=\"(.*)\">", webpage)
#   return re.findall(f"<link rel=\"{rel}\" href=\"(.*)\">", webpage)

# def insertLinkedFile(stringifiedPage: str, rel, href, webpage):
#   return re.sub(f"<link rel=\"{rel}\" href=\"{href}\">", stringifiedPage, webpage)

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
    webpage = re.sub(f"<script src=[\'\"]{script}[\'\"]>[.\s]*</script>", jsFile, webpage)
  return webpage

# compile the webpage
webpage = openFile("config.html")
webpage = replaceStyles(webpage)
webpage = replaceJavascript(webpage)

# save the compiled webpage
newFile = open(".\\data\\config.html", "w")
newFile.write(webpage)
