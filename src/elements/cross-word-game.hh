<?hh

class :cross-word-game extends :x:element {
  use XHPHelpers;
  use XHPReact;

  attribute :xhp:html-element;
  attribute Vector<CrossWordString> horizontal = Vector {};
  attribute Vector<CrossWordString> vertical = Vector {};
  attribute CrossWordGrid grid;

  protected function render(): XHPRoot {
    $blank_grid = Map {};
    $grid = $this->:grid;
    $lim = Map {};
    if ($grid !== null) {
      $limits = $grid->get_limits();
      for ($x=$limits->max_y(); $x>=$limits->min_y(); $x--) {
        $blank_grid[$x] = Map {};
        for ($y=$limits->min_x(); $y<=$limits->max_x(); $y++) {
          $cell = $grid->get_cell($y, $x);
          if ($cell !== null) {
            $blank_grid[$x][$y] = true;
          }
        }
      }
      $lim['minX'] = $limits->min_x();
      $lim['maxX'] = $limits->max_x();
      $lim['minY'] = $limits->min_y();
      $lim['maxY'] = $limits->max_y();
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
        'limits' => $lim
      },
    );

    return (
        <section id={$this->getID()} />
    );
  }
}
