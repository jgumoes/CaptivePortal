require_relative "../libs/functions.rb"

describe "#scan_networks" do
  let(:vars){ double :vars}

  it "returns No Networks string when empty" do
    allow(vars).to receive(:networks).and_return([])
    foutput = scan_networks(vars)
    puts foutput
    expect(foutput).to eq "<label class='radios'>No Networks Found"\
        "<input type='radio' name='radio' value=''>"\
        "<span class='checkmark'></span>"\
      "</label>"
    end
  end
