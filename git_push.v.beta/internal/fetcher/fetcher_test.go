package fetcher

import "testing"

func TestParseLines(t *testing.T) {
	text := `tg://proxy?server=1.2.3.4&port=443&secret=0123456789abcdef0123456789abcdef|2026-10-26T12:45:44
https://t.me/proxy?server=5.6.7.8&port=5500&secret=7HQighJPBNMYVRNB6tdkVw
9.9.9.9:8443:79e344818749bd7ac519130220c25d09
# comment
invalid line`

	list := parseLines(text)
	if len(list) != 3 {
		t.Fatalf("got %d entries, want 3", len(list))
	}

	if list[0].Host != "1.2.3.4" || list[0].Secret != "0123456789abcdef0123456789abcdef" {
		t.Fatalf("unexpected tg link parse: %+v", list[0])
	}
	if list[1].Host != "5.6.7.8" || list[1].Port != 5500 || list[1].Secret != "7HQighJPBNMYVRNB6tdkVw" {
		t.Fatalf("unexpected t.me link parse: %+v", list[1])
	}
	if list[2].Host != "9.9.9.9" || list[2].Port != 8443 {
		t.Fatalf("unexpected plain line parse: %+v", list[2])
	}
}
