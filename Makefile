
all: app

helper.a: helper.o
	ar rcs helper.a helper.o
	cp helper.a /tmp/

helper.o: helper.cu
	/usr/local/cuda-10.2/bin/nvcc \
      -m64 \
      -gencode arch=compute_53,code=sm_53 \
      -o helper.o -c helper.cu

app: app.cpp helper.o
	g++ -o app app.cpp helper.o  \
      -I/usr/local/cuda/targets/aarch64-linux/include \
      -L/usr/local/cuda/targets/aarch64-linux/lib/ \
      -lcudart 

clean:
	@rm -f *.o *.a app

.PHONY: clean
