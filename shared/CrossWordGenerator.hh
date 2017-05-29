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

    foreach($word_hint_map as $key => $word) {
      $this->words[] = new CrossWordString(strtolower($key), $word);
    }

    if ($this->words->count() > 0) {
      shuffle_vec($this->words);

      $first = $this->words[0];
      $index = 0;
      foreach ($first->get_cells() as $word) {
        $word->set_position($index, 0);
        $this->grid->set_cell($index, 0, $word);
        $index++;
      }
      $this->placed[] = $first;
      $this->words->removeKey(0);

      // echo "After removal\n";
      // print_words($this->words);

      echo "first: " . $first . "\n";
      foreach ($this->words as $word) {
        $this->join_attempt($word);
      }
    }
    $this->cross_word = new CrossWord($this->words, $this->grid);
  }

  public function get_cross_word(): CrossWord {
    return $this->cross_word;
  }

  private function join_attempt(CrossWordString $word) {
    echo "Joining " . $word->get_word() . " against\n";
    print_vector($this->placed);
    $common = $this->get_common_letters($this->placed, $word);
    // print_vector($common);

    if ($common->count() === 0) {
      throw new NoCommonLetters($word, $this->placed, $this->grid);
    }

    shuffle_vec($common);
    $word_placed = false;
    foreach ($common as $c) {
      $word_placed = $this->place_word_on_letter($c, $word);
      if ($word_placed) {
        echo "placed\n";
        break;
      }
    }

    if (!$word_placed) {
      echo "not placed\n";
      throw new AllCommonLettersOccluded($word, $this->placed, $this->grid);
    }
  }

  private function place_word_on_letter(CrossWordCell $letter, CrossWordString $word): bool {
    echo "placing '" . $word . "' on '" . $letter . "'\n";
    $neighbor_count = $this->grid->cell_neighbor_count($letter);
    if ($letter->get_join() !== null || $neighbor_count > 2) {
      return false;
    }

    echo "chosen placed letter " . $letter . "\n";
    $options = $word->get_cells_with_letter($letter->get_letter());
    $roll = rand(0, $options->count()-1);
    $join_letter = $options[$roll]; // Letter in placing word that will overlap $letter
    // echo "chosen join letter " . $join_letter . "\n";

    $align_vert = $letter->get_owner()->is_horizontal();

    $possible_multi = Vector {};
    foreach ($this->placed as $placed_word) {
      // Ignore words that are oriented same as new placed word will be
      if ($align_vert && $placed_word->is_verticle()
      || !$align_vert && $placed_word->is_horizontal()) {
        continue;
      }

      foreach ($placed_word->get_cells() as $cell) {
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
          && $neighbor->get_owner() !== $word
          && !$is_multi_join) {
            // echo "cannot place " . ($is_multi_join ? 'true' : 'false') . "\n";
            throw new CannotPlaceWord(Pair{$letter, $join_letter}, $word, $this->placed, $this->grid);
          }
        }
        $this->grid->set_cell($x, $y, $cell);
      }
    };

    $new_cells = Vector {};
    try {
      $pos_dif = $join_letter->get_position();
      if ($letter->get_owner()->is_horizontal()) {
        // echo "horizontal\n";
        $start_y = $letter->get_y() + $pos_dif;
        $x = $letter->get_x();
        foreach ($word->get_cells() as $cell) {
          $join_func($x, $start_y, $cell);
          $new_cells[] = $cell;
          $start_y--;
        }
      } else {
        // echo "vertical\n";
        $start_x = $letter->get_x() - $pos_dif;
        $y = $letter->get_y();
        foreach($word->get_cells() as $cell) {
          $join_func($start_x, $y, $cell);
          $new_cells[] = $cell;
          $start_x++;
        }
      }
    } catch (CannotPlaceWord $e) {
      foreach ($new_cells as $new_cell) {
        $join = $new_cell->get_join();
        if ($join !== null) {
          // Clear join
          $join->clear_join();
          $new_cell->clear_join();
        } else {
          // Clear grid cell
          $this->grid->clear_cell($new_cell->get_x(), $new_cell->get_y());
        }
      }
      return false;
    }

    echo "placing: " . $word . "\n";
    $this->placed[] = $word;
    return true;
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
