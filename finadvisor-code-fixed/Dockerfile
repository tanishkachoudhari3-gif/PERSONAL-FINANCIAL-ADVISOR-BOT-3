FROM debian:bookworm-slim AS build
RUN apt-get update && apt-get install -y --no-install-recommends gcc make libc6-dev ca-certificates && rm -rf /var/lib/apt/lists/*
WORKDIR /app
COPY . .
RUN make build
FROM debian:bookworm-slim
WORKDIR /app
COPY --from=build /app/bin/finadvisor /app/bin/finadvisor
COPY --from=build /app/public /app/public
EXPOSE 3000
CMD ["/app/bin/finadvisor"]
