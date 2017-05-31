FROM hhvm/hhvm-proxygen:latest

RUN apt-get update && \
    apt-get install -y curl && \
    mkdir /opt/composer && \
    curl --silent --show-error https://getcomposer.org/installer | hhvm --php -- --install-dir=/opt/composer && \
    rm -rf /var/www

ADD . /var/www
RUN cd /var/www && \
    hhvm /opt/composer/composer.phar install

EXPOSE 80
