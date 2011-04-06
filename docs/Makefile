#!/usr/bin/make

TIMESTAMP :=$(shell date)
ORGFILES := $(wildcard *.org)
HTMLFILES :=$(patsubst %.org,%.html,${ORGFILES})

all:: ${HTMLFILES}

${HTMLFILES}: ${ORGFILES} header.inc footer.inc

.PHONY: clean

clean:
	rm -rf ${HTMLFILES}
	find . -name '*~' | xargs rm -f

%.html: %.org
	emacs --batch --load=settings.el --visit=$< \
	--funcall org-export-as-html-batch
	sed -e '/<body>/r header.inc'  $@ > tmp && mv tmp $@
	sed -e '/postamble/r footer.inc' $@ | sed -e 's/DATE_HERE/${TIMESTAMP}/' > tmp && mv tmp $@
