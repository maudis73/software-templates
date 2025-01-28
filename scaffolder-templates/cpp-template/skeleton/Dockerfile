FROM redhat/ubi8

RUN yum install -y gcc-c++ 

WORKDIR /app

COPY src/SimpleRestApi.cpp . 

RUN g++ -o hello_world SimpleRestApi.cpp

EXPOSE 8080

CMD ["./hello_world"]
