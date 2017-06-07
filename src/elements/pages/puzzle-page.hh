<?hh
require_once("../src/elements/pages/base-page.hh");
require_once("../src/elements/content.hh");
require_once("../src/elements/cross-word-game.hh");

class :puzzle-page extends :base-page {

  protected function render(): XHPRoot {
    $puzzle_id = "";
    if ($this->path->containsKey(1)) {
      $puzzle_id = $this->path[1];
    }

    $cross_word = DB()->getCrossWord($puzzle_id);

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

      $horz_list = <ul><li>Horizontal</li></ul>;
      $vert_list = <ul><li>Vertical</li></ul>;

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
        for ($y=$limits->min_x(); $y<=$limits->max_x(); $y++) {
          $cell = $grid->get_cell($y, $x);
          $table_row->appendChild(
            <td class={$cell === null ? "empty" : ""}></td>
          );
        }
        $table->appendChild($table_row);
      }

      return (
        <content title="Puzzle" components={Vector {"CrossWordGame"}}>
          <h1>Solve</h1>
          <section class="cross-word">
            {$table}
          </section>
          <section class="word-hints">
            {$horz_list}
            {$vert_list}
          </section>
          <x:js-scope>
            <cross-word-game />
          </x:js-scope>
        </content>
      );
    } else {
      return <not-found message="Puzzle could not be found." />;
    }
  }
}
