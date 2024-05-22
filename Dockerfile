FROM gcc:latest as build

RUN apt-get update
RUN apt-get -y install  g++ make cmake openssl libssl-dev libpq-dev

COPY . /app

# Переход в директорию с исходным кодом
WORKDIR /app

#RUN cmake . -DENABLE_DATA_POSTGRESQL=ON
RUN cmake . && cmake --build .
#
## Определение команды для запуска приложения
#ENTRYPOINT ["./app/untitled"]
CMD ["/app/untitled"]