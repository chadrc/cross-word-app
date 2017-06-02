FROM base-cross-word-image:local

ADD . /var/www
ADD site.ini /etc/hhvm/site.ini
RUN cd /var/www && \
    hhvm /opt/composer/composer.phar install
