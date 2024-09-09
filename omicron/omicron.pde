import java.util.ArrayList;
import java.util.HashMap;
import processing.core.PFont;

HashMap<String, Float> variableValues;
ArrayList<String> expressions;
ArrayList<Float> results;
PFont pixelFont;

int siz = 7;

void setup() {
  size(800, 600);
  background(255);

  // Load pixel font
  pixelFont = createFont("/home/kof/.fonts/Monaco_Linux.ttf", siz); // Change to your pixel font file name and size
  textFont(pixelFont);

  variableValues = new HashMap<String, Float>();
  variableValues.put("a", 1.0);
  variableValues.put("b", 2.0);
  variableValues.put("c", 3.0); // Unicode character
  variableValues.put("d", 4.0); // Unicode character

  String[] vars = {"a", "b", "c", "d"};
  String[] ops = {"+", "−", "×", "÷"};

  expressions = generateExpressions(vars, ops);
  results = new ArrayList<Float>();

  for (String expr : expressions) {
    results.add(evaluateExpression(expr));
  }

  drawTable();
  drawLegend(); // Ensure the legend is drawn after the table
}

void drawTable() {
  int startX = 50;
  int startY = 50;
  int rowHeight = siz+1; // Adjusted for pixel font size
  int colWidth = 100;

  // Header
  fill(0);
  textSize(siz); // Slightly larger for header
  text("Expression", startX, startY);
  text("Result", startX + colWidth, startY);

  // Draw rows
  textSize(siz); // Pixel font size
  for (int i = 0; i < expressions.size(); i++) {
    String expr = expressions.get(i);
    Float result = results.get(i);

    text(expr, startX, startY + (i + 1) * rowHeight);
    String res = (result != null) ? result.toString() : "ERROR"; 
    text(res, startX + colWidth, startY + (i + 1) * rowHeight);
	println(expr + " = " + res);
   
  }
}

void drawLegend() {
  int legendX = 350;
  int legendY = 100;
  int legendRowHeight = siz+1; // Adjusted for pixel font size

  fill(0);
  textSize(7);
  text("Legend:", legendX, legendY);

  // Legend entries
  String[] legendLabels = {"a = 1", "b = 2", "c = 3", "d = 4", "+, −, ×, ÷"};
  for (int i = 0; i < legendLabels.length; i++) {
    text(legendLabels[i], legendX, legendY + (i + 1) * legendRowHeight);
  }

  // Omicron symbol
  fill(0, 0, 255);
  textSize(siz*2); // Larger for emphasis
  text("\u03BF", legendX, legendY + (legendLabels.length + 1) * legendRowHeight);

  // Explanation of Omicron
  fill(0);
  textSize(siz);
  text("Omicron (ο)", legendX + 30, legendY + (legendLabels.length + 1) * legendRowHeight);
  
}

ArrayList<String> generateExpressions(String[] vars, String[] ops) {
  ArrayList<String> expressions = new ArrayList<String>();
  for (String op1 : ops) {
    for (String op2 : ops) {
      for (String op3 : ops) {
        expressions.add(vars[0] + " " + op1 + " " + vars[1] + " " + op2 + " " + vars[2] + " " + op3 + " " + vars[3]);
      }
    }
  }
  return expressions;
}

Float evaluateExpression(String expr) {
  String[] tokens = expr.split(" ");
  if (tokens.length != 7) {
    return null; // Invalid expression
  }

  Float result = variableValues.get(tokens[0]);
  if (result == null) {
    return null; // Variable not found
  }

  for (int i = 1; i < tokens.length; i += 2) {
    if (i + 1 >= tokens.length) {
      return null; // Missing number after operator
    }
    String op = tokens[i];
    Float num = variableValues.get(tokens[i + 1]);
    if (num == null) {
      return null; // Variable not found
    }

    switch (op) {
      case "+":
        result += num;
        break;
      case "−": // Unicode minus
        result -= num;
        break;
      case "×":
        result *= num;
        break;
      case "÷":
        if (num != 0) {
          result /= num;
        } else {
          return null; // Division by zero
        }
        break;
      default:
        return null; // Unknown operator
    }
  }

  return result;
}
