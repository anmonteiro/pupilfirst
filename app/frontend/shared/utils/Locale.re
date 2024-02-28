type nonrec intlDisplayNames;

[@mel.send]
external intlDisplayNameOf: (intlDisplayNames, string) => string = "of";

[@mel.new]
external createIntlDisplayNames:
  (array(string), {. "type": string}) => intlDisplayNames =
  "Intl.DisplayNames";

let toLanguageName = (inputLanguage, outputLanguage) => {
  let intlDisplayNames =
    createIntlDisplayNames([|outputLanguage|], {"type": "language"});
  intlDisplayNameOf(intlDisplayNames, inputLanguage);
};

let humanize = languageCode =>
  if (languageCode->(Js.String.startsWith(~prefix="en"))) {
    toLanguageName(languageCode, languageCode);
  } else {
    (toLanguageName(languageCode, "en") ++ " - ")
    ++ toLanguageName(languageCode, languageCode);
  };
