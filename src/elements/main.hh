<?hh

class :main extends :x:element {
  attribute string title;

  protected function render(): XHPRoot {
    return (
      <html>
      <head>
        <title>{$this->:title}</title>
      </head>
      <body>
        {$this->getChildren()}
      </body>
      </html>
    );
  }
}
