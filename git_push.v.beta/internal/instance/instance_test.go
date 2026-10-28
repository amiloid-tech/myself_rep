package instance

import "testing"

func TestAcquireOnlyOnce(t *testing.T) {
	ok, err := Acquire()
	if err != nil {
		t.Fatalf("first acquire: %v", err)
	}
	if !ok {
		t.Fatal("first acquire returned false")
	}
	defer Release()

	second, err := Acquire()
	if err != nil {
		t.Fatalf("second acquire: %v", err)
	}
	if second {
		t.Fatal("second acquire returned true")
	}
}
