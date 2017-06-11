<?hh

class :cross-word-form extends :x:element {
    use XHPHelpers;
    use XHPReact;

    attribute :xhp:html-element;

    protected function render(): XHPRoot {
      $this->constructReactInstance(
        'CrossWordForm',
        Map {},
      );

      return (
          <section id={$this->getID()} />
      );
    }
}
