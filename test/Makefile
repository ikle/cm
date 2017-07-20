.PHONY: test kind

test: kind

kind:
	cd $(CURDIR)

	./kind-test name proper-na7me9
	./kind-test name Another-Name

	if ./kind-test name -inproper		; then exit 1; fi
	if ./kind-test name 899			; then exit 1; fi
	if ./kind-test name invalid--name	; then exit 1; fi
	if ./kind-test name Invalid-		; then exit 1; fi

	./kind-test number 0
	./kind-test number 348

	if ./kind-test number 00		; then exit 1; fi
	if ./kind-test number a			; then exit 1; fi

	./kind-test 'name, number' 0
