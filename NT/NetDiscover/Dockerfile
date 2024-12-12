FROM gcc:latest as build

WORKDIR /NetDiscover

COPY . .

RUN apt-get update && \
    apt-get install -y \
    make \
    && \
    make
RUN ls
FROM ubuntu:22.04

LABEL NetDiscover="admin"

WORKDIR /app

COPY --from=build /NetDiscover/netDiscover .

EXPOSE 8000
RUN ls
RUN pwd
ENTRYPOINT [ "./netDiscover" ]

