<?hh

class :cross-word-game extends :x:element {
  use XHPHelpers;
  use XHPReact;

  attribute :xhp:html-element;

  protected function render(): XHPRoot {
    $this->constructReactInstance(
      'CrossWordGame',
      Map {'value' => "my value"},
    );

    return (
        <section id={$this->getID()} />
    );
  }
}
