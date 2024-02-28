type nonrec t = {
  id: string,
  name: string,
  active: bool,
};
let id = t => t.id;
let name = t => t.name;
let active = t => t.active;
module Fragment = [%graphql
  {js|
  fragment CertificateFragment on Certificate {
    id
    name
    active
  }
|js}
];
let makeFromFragment = (certificate: Fragment.t) => {
  id: certificate.id,
  name: certificate.name,
  active: certificate.active,
};
let make = (id: string, name: string, active: bool) => {id, name, active};
