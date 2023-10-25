FROM maxermu/disp as builder

COPY ./ /src
WORKDIR /src
RUN mkdir build && cmake -DCMAKE_BUILD_TYPE:STRING=Release -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/g++-10 -S./ -B./build && \
    cd build && make -j api-gateway

FROM maxermu/disp

COPY --from=builder /src/build/api-gateway/ /app
ENTRYPOINT ["/app/api-gateway", "/app/api-gateway.yaml"]
