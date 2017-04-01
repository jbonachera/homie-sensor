snapshot: commit-version build

release: tag-version build

commit-version:
	echo "#define VERSION \"$$(git describe --tags)-$$(git rev-parse --short HEAD)\"" > src/version.h

tag-version:
	echo "#define VERSION \"$$(git describe --tags)\"" > src/version.h

build:
	docker run -v $$(pwd)/.pio_home:/root/.platformio -v $$(pwd):/usr/local/src --rm $$(docker build -q -f Dockerfile.build .)
	md5sum release/homie-intercom > release/homie-intercom.md5
	sha512sum release/homie-intercom > release/homie-intercom.sha512

ui:
	test -d data/homie || mkdir data/homie/ui_bundle.gz && curl -sL http://setup.homie-esp8266.marvinroger.fr/ui_bundle.gz -o data/homie/ui_bundle.gz
