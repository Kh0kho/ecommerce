FROM gcc

COPY ecommerce.cpp /app/
WORKDIR /app

RUN apt-get update && \
    apt-get install -y libstdc++6

RUN g++ -o myprogram ecommerce.cpp

CMD ["./myprogram"]