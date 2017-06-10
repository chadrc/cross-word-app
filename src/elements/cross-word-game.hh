<?hh

class :cross-word-game extends :x:element {
  use XHPHelpers;
  use XHPReact;

  attribute :xhp:html-element;
  attribute string puzzleId @required;
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
          $blank_grid[$x][$y] = new GridCell();
        }
      }
    }

    $horz_hints = Vector {};
    foreach ($this->:horizontal as $key => $value) {
      $first_cell = $value->get_cells()[0];
      $blank_grid[$first_cell->get_y()][$first_cell->get_x()] = new GridCell(Horizontal, $key);
      $horz_hints[] = $value->get_hint();
    }

    $vert_hints = Vector {};
    foreach ($this->:vertical as $key => $value) {
      $first_cell = $value->get_cells()[0];
      $blank_grid[$first_cell->get_y()][$first_cell->get_x()] = new GridCell(Vertical, $key);
      $vert_hints[] = $value->get_hint();
    }

    $this->constructReactInstance(
      'CrossWordGame',
      Map {
        'puzzleId' => $this->:puzzleId,
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

enum Orientation: string {
  Horizontal = "Horizontal";
  Vertical = "Vertical";
}

class GridCell {
  public function __construct(public ?Orientation $orientation = null, public ?int $wordIndex = null) {}
}

class HintWord {
  public function __construct(public string $hint, public int $firstX, public int $firstY) {}
}
