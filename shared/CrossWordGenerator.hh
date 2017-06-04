<?hh
require_once("../shared/CrossWord.hh");
require_once("../shared/WordJoinData.hh");
require_once("../shared/CrossWordGenerationExceptions.hh");

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

      // echo "first: " . $first . "\n";
      foreach ($this->words as $word) {
        $this->join_attempt($word);
      }

      $this->words[] = $first;
    }
    $this->cross_word = new CrossWord($this->words, $this->grid);
  }

  public function get_cross_word(): CrossWord {
    return $this->cross_word;
  }

  private function join_attempt(CrossWordString $word) {
    // echo "Joining " . $word->get_word() . " against\n";
    // print_vector($this->placed);
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
        // echo "placed\n";
        break;
      }
    }

    if (!$word_placed) {
      // echo "not placed\n";
      throw new AllCommonLettersOccluded($word, $this->placed, $this->grid);
    }
  }

  private function place_word_on_letter(CrossWordCell $letter, CrossWordString $word): bool {
    // echo "placing '" . $word . "' on '" . $letter . "'\n";
    $neighbor_count = $this->grid->cell_neighbor_count($letter);
    if ($letter->get_join() !== null || $neighbor_count > 2) {
      return false;
    }

    // echo "chosen placed letter " . $letter . "\n";
    $join_data = new WordJoinData($letter, $word, $this->placed);

    $new_cells = Vector {};
    try {
      $pos_dif = $join_data->get_join_letter()->get_position();
      if ($letter->get_owner()->is_horizontal()) {
        // echo "horizontal\n";
        $start_y = $letter->get_y() + $pos_dif;
        $x = $letter->get_x();
        foreach ($word->get_cells() as $cell) {
          $this->join_func($x, $start_y, $cell, $join_data);
          $new_cells[] = $cell;
          $start_y--;
        }
      } else {
        // echo "vertical\n";
        $start_x = $letter->get_x() - $pos_dif;
        $y = $letter->get_y();
        foreach($word->get_cells() as $cell) {
          $this->join_func($start_x, $y, $cell, $join_data);
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

    // echo "placing: " . $word . "\n";
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

  private function join_func(int $x, int $y, CrossWordCell $cell, WordJoinData $join_data) {
    $cell->set_position($x, $y);
    $existing = $this->grid->get_cell($x, $y);
    if ($existing !== null) {
      $existing->join($cell);
      $cell->join($existing);
    } else {
      $neighbors = $this->grid->cell_neighbors($cell);
      foreach ($neighbors as $neighbor) {
        $is_multi_join = false;
        foreach($join_data->get_multi_joins() as $mj) {
          if ($neighbor->get_owner() === $mj->get_owner()) {
            $is_multi_join = true;
          }
        }
        if ($neighbor->get_owner() !== $join_data->get_existing_letter()->get_owner()
        && $neighbor->get_owner() !== $join_data->get_word()
        && !$is_multi_join) {
          // echo "cannot place " . ($is_multi_join ? 'true' : 'false') . "\n";
          throw new CannotPlaceWord($join_data, $join_data->get_word(), $this->placed, $this->grid);
        }
      }
      $this->grid->set_cell($x, $y, $cell);
    }
  }
}
