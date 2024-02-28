type locale;
type formatDistanceOptions;
[@mel.obj]
external formatDistanceOptions:
  (~locale: locale=?, ~includeSeconds: bool=?, ~addSuffix: bool=?, unit) =>
  formatDistanceOptions;
type formatDistanceStrictOptions;
[@mel.obj]
external formatDistanceStrictOptions:
  (
    ~locale: locale=?,
    ~unit: string=?,
    ~roundingMethod: string=?,
    ~addSuffix: bool=?,
    unit
  ) =>
  formatDistanceStrictOptions;
let currentTimeZone = () => "Asia/Kolkata";
let selectedTimeFormat = () => "HH:mm";
[@mel.module "date-fns"]
external formatDistanceOpt:
  (Js.Date.t, Js.Date.t, formatDistanceOptions) => string =
  "formatDistance";
[@mel.module "date-fns"]
external formatDistanceStrictOpt:
  (Js.Date.t, Js.Date.t, formatDistanceStrictOptions) => string =
  "formatDistanceStrict";
[@mel.module "date-fns"]
external formatDistanceToNowOpt: (Js.Date.t, formatDistanceOptions) => string =
  "formatDistanceToNow";
[@mel.module "date-fns"]
external formatDistanceToNowStrictOpt:
  (Js.Date.t, formatDistanceStrictOptions) => string =
  "formatDistanceToNowStrict";
let formatDistance =
    (date, baseDate, ~includeSeconds=false, ~addSuffix=false, ()) => {
  let options = formatDistanceOptions(~includeSeconds, ~addSuffix, ());
  formatDistanceOpt(date, baseDate, options);
};
let formatDistanceStrict =
    (date, baseDate, ~addSuffix=false, ~unit=?, ~roundingMethod="round", ()) => {
  let options =
    formatDistanceStrictOptions(~addSuffix, ~unit?, ~roundingMethod, ());
  formatDistanceStrictOpt(date, baseDate, options);
};
let formatDistanceToNow = (date, ~includeSeconds=false, ~addSuffix=false, ()) => {
  let options = formatDistanceOptions(~includeSeconds, ~addSuffix, ());
  formatDistanceToNowOpt(date, options);
};
let formatDistanceToNowStrict =
    (date, ~addSuffix=false, ~unit=?, ~roundingMethod="round", ()) => {
  let options =
    formatDistanceStrictOptions(~addSuffix, ~unit?, ~roundingMethod, ());
  formatDistanceToNowStrictOpt(date, options);
};
type formatOptions;
[@mel.obj]
external formatOptions:
  (
    ~timeZone: string,
    ~locale: locale=?,
    ~useAdditionalWeekYearTokens: bool=?,
    ~useAdditionalDayOfYearTokens: bool=?,
    unit
  ) =>
  formatOptions;
[@mel.module "date-fns-tz"]
external formatTz: (Js.Date.t, string, formatOptions) => string = "format";
let format = (date, fmt) => {
  let timeZone = currentTimeZone();
  let opt = formatOptions(~timeZone, ());
  formatTz(date, fmt, opt);
};
let formatPreset = (date, ~short=false, ~year=false, ~time=false, ()) => {
  let leading = if (short) {"MMM d"} else {"MMMM d"};
  let middle = if (year) {", yyyy"} else {""};
  let trailing =
    if (time) {
      " " ++ selectedTimeFormat();
    } else {
      "";
    };
  format(date, leading ++ middle ++ trailing);
};
[@mel.module "date-fns"]
external decodeISOJs: Js.Json.t => Js.Date.t = "parseISO";
let decodeISO = json =>
  if (Js.typeof(json) == "string") {
    decodeISOJs(json);
  } else {
    raise(
      Json.Decode.DecodeError("Expected string, got " ++ Js.typeof(json)),
    );
  };
let encodeISO = date => Js.Date.toISOString(date)->Js.Json.string;
[@mel.module "date-fns"] external parseISO: string => Js.Date.t = "parseISO";
[@mel.module "date-fns"] external isPast: Js.Date.t => bool = "isPast";
[@mel.module "date-fns"] external isFuture: Js.Date.t => bool = "isFuture";
[@mel.module "date-fns"]
external differenceInSeconds: (Js.Date.t, Js.Date.t) => int =
  "differenceInSeconds";
[@mel.module "date-fns"]
external differenceInMinutes: (Js.Date.t, Js.Date.t) => int =
  "differenceInMinutes";
[@mel.module "date-fns"]
external differenceInDays: (Js.Date.t, Js.Date.t) => int = "differenceInDays";
[@mel.module "date-fns"]
external addDays: (Js.Date.t, int) => Js.Date.t = "addDays";
