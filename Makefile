ui:
	test -d data/homie || mkdir data/homie/ui_bundle.gz && curl -sL http://setup.homie-esp8266.marvinroger.fr/ui_bundle.gz -o data/homie/ui_bundle.gz
build:
	docker run --rm $$(docker build -q -f Dockerfile.build .) /usr/local/bin/get-artifact > homie-sensor
	md5sum homie-sensor > homie-sensor.md5
	sha512sum homie-sensor > homie-sensor.sha512

