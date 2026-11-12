# Copyright (C) 2026 Nicolas San Martin
#
# Copying and distribution of this file, with or without modification,
# are permitted in any medium without royalty provided the copyright
# notice and this notice are preserved.  This file is offered as-is,
# without any warranty.

.PHONY: utests itests

utests: | $(HOTL_TESTS_BUILD)
	$(MAKE) -C utests

itests: | $(HOTL_TESTS_BUILD)
	$(MAKE) -C itests

all: utests itests

tags:
	ctags --languages=c -R .

cscope:
	cscope -b -k -R

clean:
	find build -type f -delete
	if [ -f tags ]; then rm tags; fi


$(HOTL_TESTS_BUILD):
	mkdir -p $@


