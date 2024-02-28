type locale;

let parseISO: string => Js.Date.t;

let decodeISO: Js.Json.t => Js.Date.t;

let encodeISO: Js.Date.t => Js.Json.t;

let differenceInSeconds: (Js.Date.t, Js.Date.t) => int;

let differenceInMinutes: (Js.Date.t, Js.Date.t) => int;

let differenceInDays: (Js.Date.t, Js.Date.t) => int;

let addDays: (Js.Date.t, int) => Js.Date.t;

let isPast: Js.Date.t => bool;

let isFuture: Js.Date.t => bool;

let format: (Js.Date.t, string) => string;

let formatPreset:
  (Js.Date.t, ~short: bool=?, ~year: bool=?, ~time: bool=?, unit) => string;

let formatDistance:
  (Js.Date.t, Js.Date.t, ~includeSeconds: bool=?, ~addSuffix: bool=?, unit) =>
  string;

let formatDistanceStrict:
  (
    Js.Date.t,
    Js.Date.t,
    ~addSuffix: bool=?,
    ~unit: string=?,
    ~roundingMethod: string=?,
    unit
  ) =>
  string;

let formatDistanceToNow:
  (Js.Date.t, ~includeSeconds: bool=?, ~addSuffix: bool=?, unit) => string;

let formatDistanceToNowStrict:
  (
    Js.Date.t,
    ~addSuffix: bool=?,
    ~unit: string=?,
    ~roundingMethod: string=?,
    unit
  ) =>
  string;
