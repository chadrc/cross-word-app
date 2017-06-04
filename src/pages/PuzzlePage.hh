<?hh
require_once("../src/pages/BasePage.hh");
require_once("../src/pages/page.hh");

class :puzzle-page extends :base-page {

  protected function render(): XHPRoot {
    $puzzle_id = "";
    if ($this->:path-params->containsKey(1)) {
      $puzzle_id = $this->:path-params[1];
    }

    $cross_word = getCrossWord($puzzle_id);

    if ($cross_word !== null) {
      $horz_words = Vector {};
      $vert_words = Vector {};
      foreach ($cross_word->get_words() as $word) {
        if ($word->is_horizontal()) {
          $horz_words[] = $word;
        } else {
          $vert_words[] = $word;
        }
      }

      $horz_list = <ul></ul>;
      $vert_list = <ul></ul>;

      $num = 1;
      foreach ($horz_words as $word) {
        $horz_list->appendChild(
          <li>{$num}. {$word->get_word()}</li>
        );
        $num += 2;
      }

      $num = 2;
      foreach ($vert_words as $word) {
        $vert_list->appendChild(
          <li>{$num}. {$word->get_word()}</li>
        );
        $num += 2;
      }

      $grid = $cross_word->get_grid();
      $limits = $grid->get_limits();
      $table = <table></table>;
      for ($x=$limits->max_y(); $x>=$limits->min_y(); $x--) {
        $table_row = <tr></tr>;
        for ($y=$limits->min_x(); $y<$limits->max_x(); $y++) {
          $cell = $grid->get_cell($y, $x);
          $table_row->appendChild(
            <td class={$cell === null ? "empty" : ""}></td>
          );
        }
        $table->appendChild($table_row);
      }

      return (
        <page title="Puzzle">
          <section class="cross-word">
            {$table}
          </section>
          <section class="word-hints">
            {$horz_list}
            {$vert_list}
          </section>
        </page>
      );
    } else {
      return (
        <page title="Puzzle Not Found">
          <h1>We're sorry, the cross word you requested does not exist.</h1>
        </page>
      );
    }
  }
}
