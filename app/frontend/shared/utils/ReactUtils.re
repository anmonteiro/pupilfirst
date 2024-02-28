let nullUnless = (element, condition) =>
  if (condition) {element} else {React.null};
let nullIf = (element, condition) => nullUnless(element, !condition);
