FROM base-cross-word-image:local

ADD . /var/www
RUN cd /var/www && \
    hhvm /opt/composer/composer.phar install
