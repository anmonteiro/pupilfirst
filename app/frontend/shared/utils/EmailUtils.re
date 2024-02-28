let regularExpression = [%mel.re
  "/^(([^<>()\\[\\]\\.,;:\\s@\"]+(\\.[^<>()\\[\\]\\.,;:\\s@\"]+)*)|(\".+\"))@(([^<>()[\\]\\.,;:\\s@\"]+\\.)+[^<>()[\\]\\.,;:\\s@\"]{2,})$/i"
];
let isInvalid = (allowBlank, email) =>
  if (email |> String.trim |> String.length > 0) {
    !Js.Re.test(~str=email, regularExpression);
  } else {
    !allowBlank;
  };
