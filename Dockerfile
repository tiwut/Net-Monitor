FROM alpine:latest AS builder

RUN apk add --no-cache g++ make wget

WORKDIR /app

RUN wget https://raw.githubusercontent.com/yhirose/cpp-httplib/master/httplib.h
RUN wget https://raw.githubusercontent.com/nlohmann/json/develop/single_include/nlohmann/json.hpp

COPY main.cpp .
RUN g++ -O3 -std=c++11 -pthread -static main.cpp -o net_monitor

FROM alpine:latest

WORKDIR /app

RUN apk add --no-cache iputils wget tar
RUN wget -qO- https://github.com/librespeed/speedtest-cli/releases/download/v1.0.10/librespeed-cli_1.0.10_linux_amd64.tar.gz | tar xz && \
    mv librespeed-cli /usr/bin/ && \
    rm -f README.md LICENSE

COPY --from=builder /app/net_monitor .
COPY index.html .

EXPOSE 8080

CMD ["./net_monitor"]
