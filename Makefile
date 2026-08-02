.PHONY: utests scripts
utests: tags
	$(MAKE) -C utests

scripts: 
	$(MAKE) -C scripts

all: utests scripts

tags:
	ctags --languages=c -R .

cscope:
	cscope -b -k -R

clean:
	find build -type f -delete
	if [ -f tags ]; then rm tags; fi

