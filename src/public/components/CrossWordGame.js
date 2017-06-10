class CrossWordGame extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
      forceFocus: null,
      focused: null
    }
  }

  wordIndex(i, orientation) {
    if (!orientation) {
      return NaN;
    }
    if (orientation !== "Horizontal" && orientation !== "Vertical") {
      throw Error("Invalid orientation:" + orientation);
    }
    return orientation === "Horizontal" ? i * 2 + 1 : i * 2 + 2;
  }

  onKeyDown(e) {
    console.log("key press", e);
    if (e.key === "Tab") {
      console.log("tab")
      let cur = this.state.focused;
      console.log("cur", cur);
      if (!cur) {
        return;
      }

      e.preventDefault();
      e.stopPropagation();

      let xOffset = e.shiftKey ? -1 : 1;
      let yOffset = e.shiftKey ? 1 : -1;

      let east = {x: cur.x + xOffset, y: cur.y, cell: null};
      east.cell = this.props.grid[east.y][east.x];
      let south = {x: cur.x, y: cur.y + yOffset, cell: null};
      south.cell = this.props.grid[south.y] ? this.props.grid[south.y][south.x] : null;
      let next = null;
      console.log('east:', east);
      console.log('south:', south);
      if (east.cell && south.cell) {

      } else {
        next = east.cell ? east : south;
      }
      this.setState({
        forceFocus: next
      });
    }
  }

  cellFocused(x, y) {
    this.setState({
      focused: {x: x, y: y},
      forceFocus: null
    })
  }

  forceFocusCell(x, y) {
    return this.state.forceFocus && this.state.forceFocus.x === x && this.state.forceFocus.y === y;
  }

  render() {
    let rows = []
    for (let y=this.props.limits.maxY; y>=this.props.limits.minY; y--) {
      let cells = [];
      for (let x=this.props.limits.minX; x<=this.props.limits.maxX; x++) {
        let cell = this.props.grid[y][x];
        cells.push(
          <td key={`${x},${y}`} className={cell ? "" : "empty"}>
            {cell && cell.orientation ?
              <sup>{this.wordIndex(cell.wordIndex, cell.orientation)}</sup>
            : ""}
            {cell ?
              <CellInput  x={x} y={y}
                          forceFocus={this.forceFocusCell(x, y)}
                          onFocus={() => this.cellFocused(x, y)}
              />
            : ""}
          </td>
        );
      }
      rows.push(
        <tr key={y}>{cells}</tr>
      );
    }
    return (
      <section>
        <h1>Solve</h1>
        <section className="cross-word">
          <table onKeyDown={(e) => this.onKeyDown(e)}>
            <tbody>
              {rows}
            </tbody>
          </table>
        </section>
        <section className="word-hints">
          <ul>
            <li>Horizontal</li>
            {this.props.horizontal.map((item, index) => {
              return (
                <li key={`horizontal${index}`}>{this.wordIndex(index, "Horizontal")}. {item}</li>
              );
            })}
          </ul>
          <ul>
            <li>Vertical</li>
            {this.props.vertical.map((item, index) => {
              return (
                <li key={`vertical${index}`}>{this.wordIndex(index, "Vertical")}. {item}</li>
              );
            })}
          </ul>
        </section>
      </section>
    );
  }
}

window.CrossWordGame = CrossWordGame;

class CellInput extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
      letter: ""
    }
  }

  raiseOnChange(value) {
    console.log(`Change: (${this.props.x}, ${this.props.y})`);

    let letter = value.substr(0, 1);
    let overflow = value.substr(1);

    console.log(`letter: ${letter}`);
    console.log(`overflow: ${overflow}`);

    if (letter !== this.state.letter) {
      this.setState({
        letter: letter
      }, () => {
        if (this.props.onChange) {
          this.props.onChange(value);
        }
      });
    } else if (overflow && this.props.onOverflow) {
      this.props.onOverflow(overflow);
    }
  }

  raiseOnFocus() {
    console.log(`Focus: (${this.props.x}, ${this.props.y})`);

    this.input.select();

    if (this.props.onFocus) {
      this.props.onFocus();
    }
  }

  raiseOnBlur() {
    console.log(`Blur: (${this.props.x}, ${this.props.y})`)

    if (this.props.onBlur) {
      this.props.onBlur();
    }
  }

  render() {
    return (
      <input  type="text"
              tabIndex={-1}
              ref={(input) => this.input = input}
              value={this.state.letter}
              onFocus={(e) => this.raiseOnFocus()}
              onBlur={(e) => this.raiseOnBlur()}
              onChange={(e) => this.raiseOnChange(e.target.value)}
      />
    );
  }

  componentDidUpdate() {
    if (this.props.forceFocus) {
      console.log("did update force focus");
      this.input.focus();
    }
  }
}
