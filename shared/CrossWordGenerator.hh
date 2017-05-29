<?hh
require_once("shared/CrossWord.hh");

class CrossWordGenerator {
  private CrossWordGrid $grid;
  private Vector<CrossWordString> $words;
  private Vector<CrossWordString> $placed;

  private CrossWord $cross_word;

  public function __construct(Map<string, string> $word_hint_map) {
    $this->grid = new CrossWordGrid();
    $this->placed = Vector {};
    $this->words = Vector {};

    foreach($word_hint_map as $key => $value) {
      $this->words[] = new CrossWordString(strtolower($key), $value);
    }

    if ($this->words->count() > 0) {
      shuffle_vec($this->words);

      $first = $this->words[0];
      $index = 0;
      foreach ($first->get_cells() as $value) {
        $value->set_position($index, 0);
        $this->grid->set_cell($index, 0, $value);
        $index++;
      }
      $this->placed[] = $first;
      $this->words->removeKey(0);

      // echo "After removal\n";
      // print_words($this->words);

      foreach ($this->words as $value) {
        $this->join_attempt($value);
      }
    }
    $this->cross_word = new CrossWord($this->words, $this->grid);
  }

  public function get_cross_word(): CrossWord {
    return $this->cross_word;
  }

  private function join_attempt(CrossWordString $value) {
    // echo "Checking " . $value->get_word() . " against\n";
    // print_vector($this->placed);
    $common = $this->get_common_letters($this->placed, $value);
    // print_vector($common);

    if ($common->count() === 0) {
      throw new NoCommonLetters($value, $this->placed, $this->grid);
    }

    shuffle_vec($common);
    $letter = null; // Letter already in grid to be overlapped with
    foreach ($common as $c) {
      $neighbor_count = $this->grid->cell_neighbor_count($c);
      if ($c->get_join() === null && $neighbor_count <= 2) {
        $letter = $c;
        break;
      }
    }

    if ($letter === null) {
      throw new AllCommonLettersOccluded($value, $this->placed, $this->grid);
    }

    // echo "chosen placed letter " . $letter . "\n";
    $options = $value->get_cells_with_letter($letter->get_letter());
    $roll = rand(0, $options->count()-1);
    $join_letter = $options[$roll]; // Letter in placing word that will overlap $letter
    // echo "chosen join letter " . $join_letter . "\n";

    $align_vert = $letter->get_owner()->is_horizontal();

    $possible_multi = Vector {};
    foreach ($this->placed as $word) {
      // Ignore words that are oriented same as new placed word will be
      if ($align_vert && $word->is_verticle()
      || !$align_vert && $word->is_horizontal()) {
        continue;
      }

      foreach ($word->get_cells() as $cell) {
        if ($cell === $letter) {
          // Already joining on this letter
          continue;
        }

        if ($align_vert) {
          if ($cell->get_x() === $letter->get_x()) {
            $possible_multi[] = $cell;
          }
        } else {
          if ($cell->get_y() === $letter->get_y()) {
            $possible_multi[] = $cell;
          }
        }

      }
    }

    $multi_joins = Vector {};
    foreach ($possible_multi as $pm) {
      // Check if the letters that would be overlapping are the same
      // echo "checking multi " . $pm . "\n";
      $dif = NAN;
      if ($align_vert) {
        $dif = $letter->get_y() - $pm->get_y();
      } else {
        $dif = $letter->get_x() - $pm->get_x();
      }

      // echo $dif . "\n";
      $join_pos = $join_letter->get_position();
      // echo $join_pos . "\n";
      $join_index = NAN;
      if ($align_vert) {
        $join_index = $join_pos + $dif;
      } else {
        $join_index = $join_pos - $dif;
      }
      $multi_letter = $join_letter->get_owner()->get_cell($join_index);
      if ($multi_letter === null) {
        continue;
      }
      // echo $multi_letter . "\n";

      if ($multi_letter->get_letter() === $pm->get_letter()) {
        $multi_joins[] = $pm;
        // echo "is multi join\n";
      }
    }

    // Lambda to capture local state instead of passing it through to another function
    $join_func = ($x, $y, $cell) ==> {
      $cell->set_position($x, $y);
      $existing = $this->grid->get_cell($x, $y);
      if ($existing !== null) {
        $existing->join($cell);
        $cell->join($existing);
      } else {
        $neighbors = $this->grid->cell_neighbors($cell);
        foreach ($neighbors as $neighbor) {
          $is_multi_join = false;
          foreach($multi_joins as $mj) {
            if ($neighbor->get_owner() === $mj->get_owner()) {
              $is_multi_join = true;
            }
          }
          if ($neighbor->get_owner() !== $letter->get_owner()
          && $neighbor->get_owner() !== $value
          && !$is_multi_join) {
            // echo "cannot place " . ($is_multi_join ? 'true' : 'false') . "\n";
            throw new CannotPlaceWord(Pair{$letter, $join_letter}, $value, $this->placed, $this->grid);
          }
        }
        $this->grid->set_cell($x, $y, $cell);
      }
    };

    $pos_dif = $join_letter->get_position();
    if ($letter->get_owner()->is_horizontal()) {
      // echo "horizontal\n";
      $start_y = $letter->get_y() + $pos_dif;
      $x = $letter->get_x();
      foreach ($value->get_cells() as $cell) {
        $join_func($x, $start_y, $cell);
        $start_y--;
      }
    } else {
      // echo "vertical\n";
      $start_x = $letter->get_x() - $pos_dif;
      $y = $letter->get_y();
      foreach($value->get_cells() as $cell) {
        $join_func($start_x, $y, $cell);
        $start_x++;
      }
    }
    $this->placed[] = $value;
  }

  private function get_common_letters(Vector<CrossWordString> $placed, CrossWordString $placing): Vector<CrossWordCell> {
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
}

class NoCommonLetters extends CrossWordGenerationException {}
class AllCommonLettersOccluded extends CrossWordGenerationException {}

class CannotPlaceWord extends CrossWordGenerationException {
  public function __construct(
    private Pair<CrossWordCell, CrossWordCell> $join_pair,
    private CrossWordString $placing,
    private Vector<CrossWordString> $placed,
    private CrossWordGrid $grid) {
    parent::__construct($placing, $placed, $grid);
  }

  public function get_join_pair(): Pair<CrossWordCell, CrossWordCell> {
    return $this->join_pair;
  }
}

class CrossWordGenerationException extends Exception {
  public function __construct(
  private CrossWordString $placing,
  private Vector<CrossWordString> $placed,
  private CrossWordGrid $grid) {
    parent::__construct();
  }

  public function get_placing(): CrossWordString {
    return $this->placing;
  }

  public function get_placed(): Vector<CrossWordString> {
    return $this->placed;
  }

  public function get_grid(): CrossWordGrid {
    return $this->grid;
  }
}

class CrossWordCreationFailed extends Exception {
  public function __construct(private Vector<CrossWordGenerationException> $attempts) {
    parent::__construct();
  }

  public function get_attempts(): Vector<CrossWordGenerationException> {
    return $this->attempts;
  }
}
