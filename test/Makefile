.PHONY: test kind parse node

test: kind parse node

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

	# next tests requires UTF-8 locale

	# phrase from The Tale of Igor's Campaign
	./kind-test print 'Се ветри, Стрибожи внуци, веютъ с моря стрелами на храбрыя плъкы Игоревы'
	if ./kind-test print "$$(printf 'string with \n control character')"; then exit 1; fi

parse:
	cd $(CURDIR)

	[ "$$(./parse-test node.spec kind)"	= "name, number"	]
	[ "$$(./parse-test node.spec help)"	= "test node"		]
	[ "$$(./parse-test node.spec validate)"	= "run bla bla bal"	]

node:
	cd $(CURDIR)

	[ "$$(./node-print-test /   abc def 'ghi jkl' nm)" = \
	  "line = abc/def/ghi jkl/nm" ]
	[ "$$(./node-print-test ' ' abc def 'ghi jkl' 927 ' op\ "rs" tu')" = \
	  'line = abc def "ghi jkl" 927 " op\\ \"rs\" tu"' ]

	./node-validate-test spec interfaces ethernet eth0 mtu 1500

	if ./node-validate-test spec interfaces ethernet     0 mtu 1500; then exit 1; fi
	if ./node-validate-test spec interfaces ethernet    e0 mtu beep; then exit 1; fi
	if ./node-validate-test spec interfaces ethernets eth0 mtu 1500; then exit 1; fi

	[ "$$(./node-read-test conf)" = 'interfaces ethernet eth0 mtu "test value"' ]

	./node-write-test conf
	rm -rf conf/interfaces/ethernet/eth0/ttl
