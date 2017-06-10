class CrossWordGame extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
      forceFocus: null,
      focused: null,
      overflow: ""
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

      let nextCell = this.getNextCell(cur.x, cur.y, e.shiftKey ? -1 : 1, e.shiftKey ? 1 : -1)

      this.setState({
        forceFocus: nextCell.next,
        tabDirection: nextCell.direction
      });
    }
  }

  getNextCell(x, y, dirX, dirY) {
    let eastWest = {x: x + dirX, y: y, cell: null};
    eastWest.cell = this.props.grid[eastWest.y][eastWest.x];
    let northSouth = {x: x, y: y + dirY, cell: null};
    northSouth.cell = this.props.grid[northSouth.y] ? this.props.grid[northSouth.y][northSouth.x] : null;
    let next = null;
    console.log('eastWest:', eastWest);
    console.log('northSouth:', northSouth);
    if (eastWest.cell && northSouth.cell) {
      next = this.state.tabDirection === "east-west" ? eastWest : northSouth;
    } else {
      next = eastWest.cell ? eastWest : northSouth;
    }

    let dir = "";
    if (next) {
      if (next === eastWest) {
        dir = "east-west";
      } else {
        dir = "north-south";
      }
    }

    return {
      next: next,
      direction: dir
    }
  }

  cellOverflow(x, y, l) {
    let nextCell = this.getNextCell(x, y, 1, -1);
    this.setState({
      forceFocus: nextCell.next,
      overflow: l,
      tabDirection: nextCell.direction
    })
  }

  cellFocused(x, y) {
    this.setState({
      focused: {x: x, y: y},
      forceFocus: null,
      overflow: ""
    })
  }

  forceFocusCell(x, y) {
    return this.state.forceFocus && this.state.forceFocus.x === x && this.state.forceFocus.y === y;
  }

  overflowForCell(x, y) {
    return this.forceFocusCell(x, y) ? this.state.overflow : "";
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
                          overflow={this.overflowForCell(x, y)}
                          forceFocus={this.forceFocusCell(x, y)}
                          onFocus={() => this.cellFocused(x, y)}
                          onOverflow={(l) => this.cellOverflow(x, y, l)}
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

  componentWillReceiveProps(newProps) {
    if (newProps.overflow) {
      this.setState({
        letter: newProps.overflow
      });
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
              onClick={(e) => this.input.select()}
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
