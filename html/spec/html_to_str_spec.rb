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