FROM hhvm/hhvm-proxygen:latest

RUN apt-get -q update && \
    apt-get -q install -y curl && \
    apt-get -q install -y autoconf automake libtool && \
    apt-get -q install -y hhvm-dev && \
    apt-get -q install -y git && \
    git clone https://github.com/mongodb/mongo-hhvm-driver --branch master --depth 1 && \
    cd mongo-hhvm-driver/ && \
    git submodule update --init --recursive && \
    hphpize && \
    cmake . && \
    make configlib && \
    make -j 1 && \
    make install && \
    cd ../ && \
    rm -rf mongo-hhvm-driver && \
    touch etc/hhvm/site.ini && \
    echo hhvm.dynamic_extension_path=/usr/lib/x86_64-linux-gnu/hhvm/extensions/30588 >> etc/hhvm/php.ini && \
    echo hhvm.dynamic_extensions[mongodb]=mongodb.so >> etc/hhvm/php.ini && \
    mkdir /opt/composer && \
    curl -Ss https://getcomposer.org/installer | hhvm --php -- --install-dir=/opt/composer && \
    rm -rf /var/www && \
    apt-get -q -y purge hhvm-dev autoconf automake libtool git curl && \
    apt-get -q -y autoremove && \
    apt-get -q -y autoclean

EXPOSE 80
