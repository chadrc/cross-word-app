<?hh

class :cross-word-game extends :x:element {
  use XHPHelpers;
  use XHPReact;

  attribute :xhp:html-element;
  attribute Vector<CrossWordString> horizontal @required;
  attribute Vector<CrossWordString> vertical @required;
  attribute CrossWordGrid grid @required;

  protected function render(): XHPRoot {
    $blank_grid = Map {};
    $limits = $this->:grid->get_limits();
    for ($x=$limits->max_y(); $x>=$limits->min_y(); $x--) {
      $blank_grid[$x] = Map {};
      for ($y=$limits->min_x(); $y<=$limits->max_x(); $y++) {
        $cell = $this->:grid->get_cell($y, $x);
        if ($cell !== null) {
          $blank_grid[$x][$y] = true;
        }
      }
    }
    $horz_hints = $this->:horizontal->map(
      $value ==> $value->get_hint()
    );
    $vert_hints = $this->:vertical->map(
      $value ==> $value->get_hint()
    );
    $this->constructReactInstance(
      'CrossWordGame',
      Map {
        'horizontal' => $horz_hints,
        'vertical' => $vert_hints,
        'grid' => $blank_grid,
        'limits' => [
          'minX' => $limits->min_x(),
          'maxX' => $limits->max_x(),
          'minY' => $limits->min_y(),
          'maxY' => $limits->max_y()
        ]
      },
    );

    return (
        <section id={$this->getID()} />
    );
  }
}
