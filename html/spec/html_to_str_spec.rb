require "./html_to_str.rb"

# describe 'trim_tail' do
#   # trim_tail didn't work right so I replaced it with str.lstrip lol
#   it "trims the string" do
#     test_string = "  margin-bottom: 12px;"
#     (1..3).each do |n|
#       expect(trim_tail(test_string + " " * n)).to respond_with test_string
#     end
#   end
# end

describe 'include_tabs' do
  it "returns the leading whitespace" do
    test_string = "margin-bottom: 12px;"
    (1..3).each do |n|
      expect(include_tabs(" " * n + test_string)).to eq " " * n
    end
  end

  it "returns the leading indents" do
    test_string = "margin-bottom: 12px;"
    (1..3).each do |n|
      expect(include_tabs("\t" * n + " " + test_string)).to eq "\t" * n + " "
    end
  end

  it "returns nothing (spaces)" do
    test_string = "a   margin-bottom: 12px;"
    expect(include_tabs(test_string)).to eq ""
  end

  it "returns nothing (tabs)" do
    test_string = "a\t\t\tmargin-bottom: 12px;"
    expect(include_tabs(test_string)).to eq ""
  end
end

describe "trimmed_and_stringed" do
  
  result_string = '"margin-bottom: 12px;"'
  it "quotes the string" do 
    test_string = "margin-bottom: 12px;"
    expect(trimmed_and_stringed(test_string)).to eq result_string
  end

  it "trims and quotes the string" do 
    test_string = "   margin-bottom: 12px;      "
    expect(trimmed_and_stringed(test_string)).to eq result_string
  end

end

describe "parse_line" do
  
  it "parses a tabbed and commented string" do
    test_string = "  margin-bottom: 12px;  /* vertical padding between buttons */"
    result_string = "  \"margin-bottom: 12px;\"\t/* vertical padding between buttons */" + "\n"
    expect(parse_line(test_string)).to eq result_string
  end

  it "parses a different tabbed and commented string" do
    test_string = ".radios {display: block;position: relative;padding-left: 35px;/*padding for the radio button labels*/"
    result_string = "\".radios {display: block;position: relative;padding-left: 35px;\"\t/*padding for the radio button labels*/" + "\n"
    expect(parse_line(test_string)).to eq result_string
  end

  it "parses a tabbed uncommented string"do
    test_string = "  cursor: pointer;font-family: Helvetica;font-size: 22px;}"
    result_string = '  "cursor: pointer;font-family: Helvetica;font-size: 22px;}"' + "\n"
    expect(parse_line(test_string)).to eq result_string
  end

  it "parses an un-indented string" do
    test_string = "<!DOCTYPE html>"
    result_string = '"<!DOCTYPE html>"' + "\n"
    expect(parse_line(test_string)).to eq result_string
  end

  it "does nothing to an indented comment" do
    test_string = "  /*Advanced CSS*/"
    expect(parse_line(test_string)).to eq test_string + "\n"
  end

  it "parses an un-indented string with a comment" do
    test_string = "<!DOCTYPE html>  /*Advanced CSS*/"
    result_string = "\"<!DOCTYPE html>\"\t/*Advanced CSS*/"
    expect(parse_line(test_string)).to eq result_string + "\n"
  end

  it "returns a single line when passed an empty string" do
    expect(parse_line("")).to eq ""
  end
end

