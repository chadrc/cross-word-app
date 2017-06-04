<?hh

class :content extends :x:element {
  attribute string title;

  protected function render(): XHPRoot {
    return (
      <html>
      <head>
        <title>{$this->:title}</title>
        <link href="https://fonts.googleapis.com/css?family=Roboto|Roboto+Slab" rel="stylesheet" />
        <link rel="stylesheet" href="main.css" />
      </head>
      <body>
        <main>
          {$this->getChildren()}
        </main>
      </body>
      </html>
    );
  }
}
