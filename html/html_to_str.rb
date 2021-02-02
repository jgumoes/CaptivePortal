# a simple script that reads a .html file, and puts all the html bits
# between double quotes. puts the result into a text document.
# this is to make life easier when tweaking an .html file that will
# be served by an ESP microcontroller. the script removes as much
# whitespace as possible (i.e. indentations), but the indentations
# are reinserted by default.
#
#     i.e. |<form action='/submit'> /*form to submit settings*/
#  	       |  <h3 class='h'>Available Networks:</h3>
#          |  <h4 class='h'>(refresh if any are missing)</h4>
#
# becomes: |"<form action='/submit'> /*form to submit settings*/
#  	       |  "<h3 class='h'>Available Networks:</h3>"
#          |  "<h4 class='h'>(refresh if any are missing)</h4>"
#
# usage: ruby html_to_str.rb [filename] [options]
# filename defaults to "test config page.html" if none is given
# options:
#   -keep_shape [true/false]      if false, won't reinsert indentation
#   -file_out                     the output file to write to. set to
#                                 str_out.txt by default

$filename = "../CaptivePortal/test config page.html"
$file_out = "str_out.txt"
keep_shape = true # keep shape of html document? i.e. reinsert the whitespace at the begining of each line?
$file_buffer = ""

def trim_tail string
  i = 1
  while string[-i].match? /\S/
    i += 1
  end
  string[1..-i]
end

def include_tabs line
  # used to create the initial string for parse_line
  if keep_shape == true
    return line.match(/(?<tabs>^\s*)/)[:tabs]
  else
    return ""
  end
end

def trimmed_and_stringed string
  return "\"#{trim_tail(string)}\""
end

def parse_line line
  str_out = include_tabs(line)
  parts = line.match /(?:^\s*)(?<line>.*)(?<comment>\/\*.*\*\/)/
  if parts != nil
    str_out += trimmed_and_stringed(part[:line]) + "\t" + parts[:comment]
  else
    str_out += trimmed_and_stringed(line)
  end
  str_out
end

def load_file(filename = $filename)
  File.open(filename, "r") do |html_file|
    html_file.each do |line|
      $file_buffer.append(line)
    end
  end
end

def save_file(file_out = $file_out)
  File.open(file_out, "w") { |file| file.write $file_buffer }
end

load_file