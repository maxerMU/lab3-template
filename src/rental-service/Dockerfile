FROM maxermu/disp as builder

COPY ./ /src
WORKDIR /src
RUN mkdir build && cmake -DCMAKE_BUILD_TYPE:STRING=Release -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/g++-10 -S./ -B./build && \
    cd build && make -j rental-service

FROM maxermu/disp

COPY --from=builder /src/build/rental-service/ /app
ENTRYPOINT ["/app/rental-service", "/app/rents.yaml"]
