<?hh

class CrossWord {
  public function __construct(private Vector<CrossWordString> $strings, private Map<int, Map<int, CrossWordCell>> $grid) {

  }

  public function get_grid(): Map<int, Map<int, CrossWordCell>> {
    return $this->grid;
  }
}

class CrossWordCell {
  private ?CrossWordCell $top = null;
  private ?CrossWordCell $right = null;
  private ?CrossWordCell $bottom = null;
  private ?CrossWordCell $left = null;

  private ?CrossWordCell $join = null;

  private int $x = -10000;
  private int $y = -10000;

  public function __construct(private CrossWordString $owner, private int $position, private string $letter) {

  }

  public function set_position(int $x, int $y) {
    $this->x = $x;
    $this->y = $y;
  }

  public function get_letter(): string {
    return $this->letter;
  }

  public function get_owner(): CrossWordString {
    return $this->owner;
  }

  public function get_position(): int {
    return $this->position;
  }

  public function get_x(): int {
    return $this->x;
  }

  public function get_y(): int {
    return $this->y;
  }

  public function join(CrossWordCell $cell) {
    $this->join = $cell;
  }

  public function get_join(): ?CrossWordCell {
    return $this->join;
  }

  public function __toString(): string {
    return $this->letter . " at " . $this->position . " in " . $this->owner->get_word();
  }
}

class CrossWordString {
  private Vector<CrossWordCell> $cells = Vector {};

  public function __construct(private string $word, private string $hint) {
    $strlen = strlen( $word );
    for( $i = 0; $i <= $strlen-1; $i++ ) {
        $char = substr( $word, $i, 1 );
        $this->cells[] = new CrossWordCell($this, $i, $char);
    }
  }

  public function get_cells(): Vector<CrossWordCell> {
    return $this->cells;
  }

  public function get_word(): string {
    return $this->word;
  }

  public function contains_letter(string $letter): bool {
    foreach ($this->cells as $cell) {
      if ($cell->get_letter() === $letter) {
        return true;
      }
    }
    return false;
  }

  public function get_cells_with_letter(string $letter): Vector<CrossWordCell> {
    $cells = Vector {};
    foreach ($this->cells as $cell) {
      if ($cell->get_letter() === $letter) {
        $cells[] = $cell;
      }
    }
    return $cells;
  }

  public function is_horizontal(): bool {
    return $this->cells[0]->get_y() === $this->cells[1]->get_y();
  }

  public function is_verticle(): bool {
    return $this->cells[0]->get_x() === $this->cells[1]->get_x();
  }

  public function __toString(): string {
    return $this->word;
  }
}

function create_cross_word(Map<string, string> $wordHintMap): CrossWord {
  $words = Vector {};
  foreach($wordHintMap as $key => $value) {
    $words[] = new CrossWordString(strtolower($key), $value);
  }

  print_words($words);

  $placed = Vector {};
  $grid = Map {};

  if ($words->count() > 0) {
    $first = $words[0];
    $index = 0;
    foreach ($first->get_cells() as $value) {
      $value->set_position($index, 0);
      $grid[$index] = Map {};
      $grid[$index][0] = $value;
      $index++;
    }
    $placed[] = $first;
    $words->removeKey(0);

    echo "After removal\n";
    print_words($words);

    foreach ($words as $value) {
      echo "Checking " . $value->get_word() . " against\n";
      print_words($placed);
      $common = get_common_letters($placed, $value);
      print_letters($common);
      if ($common->count() > 0) {
        shuffle_letters($common);
        $letter = $common[0]; // Letter already in grid to be overlapped with
        echo "chosen placed letter " . $letter . "\n";
        $options = $value->get_cells_with_letter($letter->get_letter());
        $roll = rand(0, $options->count()-1);
        $join_letter = $options[$roll]; // Letter in placing word that will overlap $letter
        echo "chosen join letter " . $join_letter . "\n";

        $pos_dif = $join_letter->get_position();
        if ($letter->get_owner()->is_horizontal()) {
          echo "horizontal\n";
          $start_y = $letter->get_y() + $pos_dif;
          $x = $letter->get_x();
          if (!$grid->containsKey($letter->get_x())) {
            $grid[$x] = Map {};
          }

          $col = $grid[$x];
          foreach ($value->get_cells() as $cell) {
            if ($col->containsKey($start_y)) {
              $col[$start_y]->join($cell);
            } else {
              $col[$start_y] = $cell;
            }
            $cell->set_position($x, $start_y);
            $start_y--;
          }
        } else {
          echo "vertical\n";
          $start_x = $letter->get_x() - ($pos_dif + 1);
          $y = $letter->get_y();

          foreach($value->get_cells() as $cell) {
            if (!$grid.containsKey($start_x)) {
              $grid[$start_x] = Map {};
            }

            $col = $grid[$start_x];
            if ($col.containsKey($y)) {
              $col[$y]->join($cell);
            } else {
              $col[$y] = $cell;
            }
            $cell->set_position($start_x, $y);
            $start_x++;
          }
        }

        $placed[] = $value;
      }
    }
  }

  return new CrossWord($words, $grid);
}

function get_common_letters(Vector<CrossWordString> $placed, CrossWordString $placing): Vector<CrossWordCell> {
  $common = Vector {};
  foreach ($placed as $word) {
    foreach ($word->get_cells() as $cell) {
      if ($placing->contains_letter($cell->get_letter())) {
        $common[] = $cell;
      }
    }
  }
  return $common;
}

function shuffle_letters(Vector<CrossWordCell> $letters) {
  for ($i=0; $i<$letters->count(); $i++) {
    $roll = rand($i, $letters->count()-1);
    $temp = $letters[$i];
    $letters[$i] = $letters[$roll];
    $letters[$roll] = $temp;
  }
}

function print_words(Vector<CrossWordString> $words) {
  foreach ($words as $value) {
    echo $value . "\n";
  }
}

function print_letters(Vector<CrossWordCell> $letters) {
  foreach ($letters as $value) {
    echo $value . "\n";
  }
}

function print_grid(Map<int, Map<int, CrossWordCell>> $grid) {
  $lower_x = PHP_INT_MAX;
  $upper_x = PHP_INT_MIN;
  $lower_y = PHP_INT_MAX;
  $upper_y = PHP_INT_MIN;

  foreach ($grid as $key => $string) {
    foreach ($string as $key => $letter) {
      if ($letter->get_x() > $upper_x) {
        $upper_x = $letter->get_x();
      }

      if ($letter->get_x() < $lower_x) {
        $lower_x = $letter->get_x();
      }

      if ($letter->get_y() > $upper_y) {
        $upper_y = $letter->get_y();
      }

      if ($letter->get_y() < $lower_y) {
        $lower_y = $letter->get_y();
      }
    }

    for ($col=$lower_x; $col<$upper_x; $col++) {
      $col_cells = null;
      try {
        $col_cells = $grid[$col];
      } catch (Exception $e) {
        continue;
      }
      echo "| ";
      for ($row=$upper_y; $row>$lower_y; $row--) {
        $row_cell = null;
        try {
          $row_cell = $col_cells[$row];
        } catch (Exception $e) {
          echo "  | ";
          continue;
        }
        echo $row_cell->get_letter() . " | ";
      }
      echo " |\n";
    }
  }
}
