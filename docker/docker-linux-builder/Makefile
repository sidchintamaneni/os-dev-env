BASE_PROJ ?= $(realpath $(shell pwd)/../../)
LINUX ?= ${BASE_PROJ}/linux

docker: Dockerfile
	docker buildx build --network=host --progress=plain -t runtime .

linux:
	docker run --rm -v ${LINUX}:/linux linux-builder make -j`nproc` bzImage 

linux-clean:
	docker run -v ${LINUX}:/linux linux-builder make clean

