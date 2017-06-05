<?hh

class :content extends :x:element {
  attribute string title;

  protected function render(): XHPRoot {
    return (
      <html>
      <head>
        <title>{$this->:title}</title>
        <script src="https://cdnjs.cloudflare.com/ajax/libs/react/15.5.4/react.js"></script>
        <script src="https://cdnjs.cloudflare.com/ajax/libs/react/15.5.4/react-dom.js"></script>
        <link href="https://fonts.googleapis.com/css?family=Roboto|Roboto+Slab" rel="stylesheet" />
        <link rel="stylesheet" href="/main.css" />
      </head>
      <body>
        {requestUri() === "/" ? "" :
          <nav>
            <a href="/">Home</a>
            <a href="/create">Create</a>
            <a href="/puzzles">Play</a>
          </nav>
        }
        <main>
          {$this->getChildren()}
        </main>
      </body>
      </html>
    );
  }
}
